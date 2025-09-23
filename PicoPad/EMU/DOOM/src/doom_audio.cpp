#include "../include.h"

#include "engine/doom_port.h"

extern "C" {
#include "engine/chocolate/deh_str.h"
#include "engine/chocolate/i_picosound.h"
#include "engine/chocolate/i_sound.h"
#include "engine/chocolate/m_misc.h"
#include "engine/chocolate/w_wad.h"
#include "engine/chocolate/z_zone.h"
}

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstring>

#include "pico/critical_section.h"

namespace
{

constexpr int kSampleRate = DOOM_AUDIO_SAMPLE_RATE;
constexpr size_t kBufferSamples = DOOM_AUDIO_BUFFER;
constexpr int kMasterVolumeMax = 256;
constexpr int kFadeStep = 8;
constexpr uint32_t kPositionFracBits = 16;
constexpr uint32_t kPositionFracMask = (1u << kPositionFracBits) - 1u;

struct PicoSfxData
{
        lumpindex_t lumpnum = -1;
        int samplerate = 0;
        int length = 0;
        int8_t *samples = nullptr;
        PicoSfxData *next = nullptr;
};

struct PicoSfxEntry
{
        const sfxinfo_t *info = nullptr;
        PicoSfxData *data = nullptr;
};

struct PicoChannel
{
        PicoSfxData *data = nullptr;
        uint32_t position = 0;
        uint32_t step = 0;
        int volume = 0;
        bool active = false;
};

critical_section_t audio_lock;
bool audio_lock_initialised = false;
bool audio_initialised = false;
bool use_sfx_prefix = false;
bool stream_started = false;

std::array<PicoChannel, NUM_SOUND_CHANNELS> channels{};
PicoSfxEntry *sfx_entries = nullptr;
int sfx_entry_count = 0;
PicoSfxData *sfx_data_head = nullptr;

std::array<std::array<uint8_t, kBufferSamples>, 2> stream_buffers{};
int stream_write_index = 0;

int master_volume = kMasterVolumeMax;
int target_volume = kMasterVolumeMax;
int fade_step = 0;

struct AudioGuard
{
        AudioGuard()
        {
                if (audio_lock_initialised)
                {
                        critical_section_enter_blocking(&audio_lock);
                }
        }

        ~AudioGuard()
        {
                if (audio_lock_initialised)
                {
                        critical_section_exit(&audio_lock);
                }
        }
};

const sfxinfo_t *ResolveSfxLink(const sfxinfo_t *sfx)
{
        while (sfx->link != nullptr)
        {
                sfx = sfx->link;
        }
        return sfx;
}

void BuildLumpName(const sfxinfo_t *sfx, char *buffer, size_t buffer_len)
{
        const sfxinfo_t *base = ResolveSfxLink(sfx);
        const char *name = DEH_String(base->name);

        if (use_sfx_prefix)
        {
                M_snprintf(buffer, buffer_len, "ds%s", name);
        }
        else
        {
                M_StringCopy(buffer, name, buffer_len);
        }

        // Lumps are stored uppercase; normalise to avoid mismatches.
        for (size_t i = 0; i < buffer_len && buffer[i] != '\0'; ++i)
        {
                buffer[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(buffer[i])));
        }
}

PicoSfxData *FindSfxData(lumpindex_t lumpnum)
{
        for (PicoSfxData *node = sfx_data_head; node != nullptr; node = node->next)
        {
                if (node->lumpnum == lumpnum)
                {
                        return node;
                }
        }
        return nullptr;
}

PicoSfxData *LoadSfxData(lumpindex_t lumpnum)
{
        if (lumpnum < 0)
        {
                return nullptr;
        }

        if (PicoSfxData *existing = FindSfxData(lumpnum))
        {
                return existing;
        }

        const uint8_t *raw = static_cast<const uint8_t *>(W_CacheLumpNum(lumpnum, PU_STATIC));
        const int lump_length = W_LumpLength(lumpnum);

        if (raw == nullptr || lump_length < 8)
        {
                W_ReleaseLumpNum(lumpnum);
                return nullptr;
        }

        const int samplerate = (raw[2] | (raw[3] << 8));
        const int sample_count = (raw[4] | (raw[5] << 8) | (raw[6] << 16) | (raw[7] << 24));

        if (samplerate <= 0 || sample_count <= 0 || sample_count > lump_length - 8)
        {
                W_ReleaseLumpNum(lumpnum);
                return nullptr;
        }

        int8_t *samples = static_cast<int8_t *>(Z_Malloc(sample_count, PU_STATIC, nullptr));
        if (samples == nullptr)
        {
                W_ReleaseLumpNum(lumpnum);
                return nullptr;
        }

        const uint8_t *src = raw + 8;
        for (int i = 0; i < sample_count; ++i)
        {
                samples[i] = static_cast<int8_t>(static_cast<int>(src[i]) - 128);
        }

        W_ReleaseLumpNum(lumpnum);

        PicoSfxData *data = static_cast<PicoSfxData *>(Z_Malloc(sizeof(PicoSfxData), PU_STATIC, nullptr));
        if (data == nullptr)
        {
                return nullptr;
        }

        data->lumpnum = lumpnum;
        data->samplerate = samplerate;
        data->length = sample_count;
        data->samples = samples;
        data->next = sfx_data_head;
        sfx_data_head = data;
        return data;
}

PicoSfxEntry *FindSfxEntry(const sfxinfo_t *sfx)
{
        if (sfx_entries == nullptr)
        {
                return nullptr;
        }

        const sfxinfo_t *base = ResolveSfxLink(sfx);
        for (int i = 0; i < sfx_entry_count; ++i)
        {
                if (sfx_entries[i].info == base)
                {
                        return &sfx_entries[i];
                }
        }
        return nullptr;
}

void UpdateFade()
{
        if (fade_step == 0)
        {
                return;
        }

        master_volume += fade_step;
        if (fade_step > 0)
        {
                if (master_volume >= target_volume)
                {
                        master_volume = target_volume;
                        fade_step = 0;
                }
        }
        else
        {
                if (master_volume <= target_volume)
                {
                        master_volume = target_volume;
                        fade_step = 0;
                }
        }
}

void MixInto(uint8_t *dest, size_t samples)
{
        for (size_t i = 0; i < samples; ++i)
        {
                int32_t mixed = 0;

                for (auto &channel : channels)
                {
                        if (!channel.active || channel.data == nullptr)
                        {
                                continue;
                        }

                        const int length = channel.data->length;
                        if (length <= 0)
                        {
                                channel.active = false;
                                continue;
                        }

                        const uint32_t position = channel.position;
                        const int index = static_cast<int>(position >> kPositionFracBits);

                        if (index >= length)
                        {
                                channel.active = false;
                                continue;
                        }

                        const int next_index = std::min(index + 1, length - 1);
                        const int32_t sample0 = channel.data->samples[index];
                        const int32_t sample1 = channel.data->samples[next_index];
                        const int32_t frac = static_cast<int32_t>(position & kPositionFracMask);
                        const int32_t sample = sample0 + ((sample1 - sample0) * frac >> kPositionFracBits);

                        mixed += sample * channel.volume;

                        channel.position = position + channel.step;
                        if ((channel.position >> kPositionFracBits) >= static_cast<uint32_t>(length))
                        {
                                channel.active = false;
                        }
                }

                int32_t scaled = mixed;
                scaled = (scaled * master_volume) / (kMasterVolumeMax * std::max(1, NUM_SOUND_CHANNELS));
                scaled = std::clamp(scaled, -32768, 32767);
                dest[i] = static_cast<uint8_t>((scaled >> 8) + 128);
        }
}

void QueueStreamBuffer()
{
        uint8_t *buffer = stream_buffers[stream_write_index].data();
        std::fill(buffer, buffer + kBufferSamples, 128);
        MixInto(buffer, kBufferSamples);
        SoundStreamSetNext(0, buffer, static_cast<int>(kBufferSamples));
        stream_write_index ^= 1;
}

void EnsureStreamStarted()
{
        if (stream_started)
        {
                return;
        }

        for (auto &buffer : stream_buffers)
        {
                std::fill(buffer.begin(), buffer.end(), 128);
        }

        MixInto(stream_buffers[0].data(), kBufferSamples);
        MixInto(stream_buffers[1].data(), kBufferSamples);
        PlaySoundChan(0, stream_buffers[0].data(), static_cast<int>(kBufferSamples), SNDREPEAT_STREAM, 1.0f, 1.0f, SNDFORM_PCM, 0);
        SoundStreamSetNext(0, stream_buffers[1].data(), static_cast<int>(kBufferSamples));
        stream_write_index = 0;
        stream_started = true;
}

uint32_t ComputeStep(const PicoSfxData *data, int pitch)
{
        if (data == nullptr || data->samplerate <= 0)
        {
                return 0;
        }

        const float base_rate = static_cast<float>(data->samplerate);
        const float pitch_scale = std::max(1.0f / 8.0f, static_cast<float>(pitch) / static_cast<float>(NORM_PITCH));
        const float target = base_rate * pitch_scale / static_cast<float>(kSampleRate);
        const uint32_t step = static_cast<uint32_t>(std::max(target, 0.00012207f) * static_cast<float>(1u << kPositionFracBits));
        return step == 0 ? 1u : step;
}

int ComputeVolume(int vol, int sep)
{
        // Collapse stereo separation into a mono gain.
        const int left = ((254 - sep) * vol) / 127;
        const int right = (sep * vol) / 127;
        return std::max(0, (left + right) / 2);
}

} // namespace

static boolean pico_sound_init(boolean use_prefix)
{
        use_sfx_prefix = use_prefix;
        if (!audio_lock_initialised)
        {
                critical_section_init(&audio_lock);
                audio_lock_initialised = true;
        }

        {
                AudioGuard guard;
                PWMSndInit();
                for (auto &channel : channels)
                {
                        channel = PicoChannel{};
                }
                stream_started = false;
                master_volume = kMasterVolumeMax;
                target_volume = kMasterVolumeMax;
                fade_step = 0;
                audio_initialised = true;
        }

        return true;
}

static void pico_sound_shutdown(void)
{
        AudioGuard guard;
        if (!audio_initialised)
        {
                return;
        }
        PWMSndTerm();
        audio_initialised = false;
        stream_started = false;
}

static int pico_sound_get_lump(should_be_const sfxinfo_t *sfxinfo)
{
        char name[9] = {0};
        BuildLumpName(sfxinfo, name, sizeof(name));
        return W_GetNumForName(name);
}

static void pico_sound_update(void)
{
        AudioGuard guard;
        if (!audio_initialised)
        {
                return;
        }

        EnsureStreamStarted();
        UpdateFade();

        while (SoundStreamIsEmpty(0))
        {
                QueueStreamBuffer();
        }
}

static void pico_sound_update_params(int channel_index, int vol, int sep)
{
        AudioGuard guard;
        if (!audio_initialised || channel_index < 0 || channel_index >= NUM_SOUND_CHANNELS)
        {
                return;
        }

        PicoChannel &channel = channels[channel_index];
        if (!channel.active)
        {
                return;
        }

        channel.volume = ComputeVolume(vol, sep);
}

static int pico_sound_start(should_be_const sfxinfo_t *sfxinfo, int channel_index, int vol, int sep, int pitch)
{
        AudioGuard guard;
        if (!audio_initialised || channel_index < 0 || channel_index >= NUM_SOUND_CHANNELS)
        {
                return -1;
        }

        PicoSfxEntry *entry = FindSfxEntry(sfxinfo);
        if (entry == nullptr || entry->data == nullptr)
        {
                return -1;
        }

        PicoChannel &channel = channels[channel_index];
        channel.data = entry->data;
        channel.position = 0;
        channel.step = ComputeStep(entry->data, pitch);
        channel.volume = ComputeVolume(vol, sep);
        channel.active = true;
        return channel_index;
}

static void pico_sound_stop(int channel_index)
{
        AudioGuard guard;
        if (!audio_initialised || channel_index < 0 || channel_index >= NUM_SOUND_CHANNELS)
        {
                return;
        }

        channels[channel_index] = PicoChannel{};
}

static boolean pico_sound_playing(int channel_index)
{
        AudioGuard guard;
        if (!audio_initialised || channel_index < 0 || channel_index >= NUM_SOUND_CHANNELS)
        {
                return false;
        }

        return channels[channel_index].active;
}

static void pico_sound_cache(should_be_const sfxinfo_t *sounds, int num_sounds)
{
        if (num_sounds <= 0)
        {
                return;
        }

        sfx_entries = static_cast<PicoSfxEntry *>(Z_Malloc(num_sounds * sizeof(PicoSfxEntry), PU_STATIC, nullptr));
        if (sfx_entries == nullptr)
        {
                sfx_entry_count = 0;
                return;
        }

        sfx_entry_count = num_sounds;

        for (int i = 0; i < num_sounds; ++i)
        {
                PicoSfxEntry &entry = sfx_entries[i];
                entry.info = ResolveSfxLink(&sounds[i]);
                entry.data = nullptr;

                char name[9] = {0};
                BuildLumpName(entry.info, name, sizeof(name));
                lumpindex_t lumpnum = W_CheckNumForName(name);
                if (lumpnum != -1)
                {
                        entry.data = LoadSfxData(lumpnum);
                }
        }
}

static snddevice_t pico_sound_devices[] = { SNDDEVICE_SB };

extern "C" sound_module_t sound_pico_module = {
        pico_sound_devices,
        static_cast<int>(sizeof(pico_sound_devices) / sizeof(pico_sound_devices[0])),
        pico_sound_init,
        pico_sound_shutdown,
        pico_sound_get_lump,
        pico_sound_update,
        pico_sound_update_params,
        pico_sound_start,
        pico_sound_stop,
        pico_sound_playing,
        pico_sound_cache,
};

extern "C" void I_PicoSoundFade(boolean fade_in)
{
        if (!audio_initialised)
        {
                return;
        }

        AudioGuard guard;
        target_volume = fade_in ? kMasterVolumeMax : 0;
        if (master_volume == target_volume)
        {
                fade_step = 0;
        }
        else
        {
                fade_step = (target_volume > master_volume) ? kFadeStep : -kFadeStep;
        }
}

extern "C" boolean I_PicoSoundFading(void)
{
        if (!audio_initialised)
        {
                return false;
        }

        AudioGuard guard;
        return master_volume != target_volume;
}

