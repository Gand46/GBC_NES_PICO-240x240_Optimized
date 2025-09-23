#include "doom_port.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chocolate/d_event.h"
#include "chocolate/d_ticcmd.h"
#include "chocolate/i_input.h"
#include "chocolate/i_sound.h"
#include "chocolate/i_system.h"
#include "chocolate/i_timer.h"
#include "chocolate/i_video.h"
#include "chocolate/m_argv.h"
#include "chocolate/m_misc.h"
#include "chocolate/z_zone.h"

#include "pico/stdlib.h"
#include "pico/time.h"

// --- System layer -----------------------------------------------------------

#ifndef DOOM_ZONE_SIZE
#define DOOM_ZONE_SIZE          (512 * 1024)
#endif

static byte doom_zone[DOOM_ZONE_SIZE];
static ticcmd_t base_ticcmd;
static atexit_func_t exit_callbacks[8];
static boolean exit_callbacks_on_error[8];
static int exit_callback_count;
static volatile uint64_t timer_origin_us;

static void run_exit_callbacks(boolean error)
{
        for (int i = exit_callback_count - 1; i >= 0; --i)
        {
                if (!error || exit_callbacks_on_error[i])
                {
                        exit_callbacks[i]();
                }
        }
}

void I_Init(void)
{
        timer_origin_us = time_us_64();
}

byte *I_ZoneBase(int *size)
{
        if (size != NULL)
        {
                *size = DOOM_ZONE_SIZE;
        }
        return doom_zone;
}

boolean I_ConsoleStdout(void)
{
        return true;
}

ticcmd_t *I_BaseTiccmd(void)
{
        memset(&base_ticcmd, 0, sizeof(base_ticcmd));
        return &base_ticcmd;
}

void I_Quit(void)
{
        run_exit_callbacks(false);
        exit(0);
}

static void vpanic(const char *error, va_list args)
{
        vprintf(error, args);
        printf("\n");
        run_exit_callbacks(true);
        panic("DOOM fatal error");
}

void I_Error(const char *error, ...)
{
        va_list args;
        va_start(args, error);
        vpanic(error, args);
        va_end(args);
}

void I_Tactile(int on, int off, int total)
{
        (void)on;
        (void)off;
        (void)total;
}

void *I_Realloc(void *ptr, size_t size)
{
        void *res = realloc(ptr, size);
        if (size != 0 && res == NULL)
        {
                panic("I_Realloc failed");
        }
        return res;
}

boolean I_GetMemoryValue(unsigned int offset, void *value, int size)
{
        (void)offset;
        (void)value;
        (void)size;
        return false;
}

void I_AtExit(atexit_func_t func, boolean run_if_error)
{
        if (exit_callback_count < (int)arrlen(exit_callbacks))
        {
                exit_callbacks[exit_callback_count] = func;
                exit_callbacks_on_error[exit_callback_count] = run_if_error;
                ++exit_callback_count;
        }
}

void I_BindVariables(void)
{
        // The Pico build does not expose runtime configuration via cvars.
}

void I_PrintStartupBanner(const char *gamedescription)
{
        I_PrintDivider();
        I_PrintBanner(gamedescription);
        I_PrintDivider();
}

void I_PrintBanner(const char *text)
{
        printf("%s\n", text);
}

void I_PrintDivider(void)
{
        printf("==============================================\n");
}

// --- Timer layer ------------------------------------------------------------

static inline uint32_t elapsed_us(void)
{
        uint64_t now = time_us_64();
        return (uint32_t)(now - timer_origin_us);
}

int I_GetTime(void)
{
        uint32_t ms = elapsed_us() / 1000u;
        return (ms * TICRATE) / 1000u;
}

int I_GetTimeMS(void)
{
        return (int)(elapsed_us() / 1000u);
}

void I_Sleep(int ms)
{
        sleep_ms(ms);
}

void I_InitTimer(void)
{
        timer_origin_us = time_us_64();
}

void I_WaitVBL(int count)
{
        if (count > 0)
        {
                sleep_ms((uint32_t)count * 1000u / 70u);
        }
}

// --- Audio layer ------------------------------------------------------------

static boolean pico_sound_init(boolean use_sfx_prefix)
{
        (void)use_sfx_prefix;
        return false;
}

static void pico_sound_shutdown(void)
{
}

static int pico_sound_get_lump(should_be_const sfxinfo_t *sfxinfo)
{
        (void)sfxinfo;
        return 0;
}

static void pico_sound_update(void)
{
}

static void pico_sound_update_params(int channel, int vol, int sep)
{
        (void)channel;
        (void)vol;
        (void)sep;
}

static int pico_sound_start(should_be_const sfxinfo_t *sfxinfo, int channel, int vol, int sep, int pitch)
{
        (void)sfxinfo;
        (void)channel;
        (void)vol;
        (void)sep;
        (void)pitch;
        return -1;
}

static void pico_sound_stop(int channel)
{
        (void)channel;
}

static boolean pico_sound_playing(int channel)
{
        (void)channel;
        return false;
}

static void pico_sound_cache(should_be_const sfxinfo_t *sounds, int num_sounds)
{
        (void)sounds;
        (void)num_sounds;
}

static snddevice_t pico_sound_devices[] = { SNDDEVICE_SB };

sound_module_t sound_pico_module = {
        .sound_devices = pico_sound_devices,
        .num_sound_devices = (int)arrlen(pico_sound_devices),
        .Init = pico_sound_init,
        .Shutdown = pico_sound_shutdown,
        .GetSfxLumpNum = pico_sound_get_lump,
        .Update = pico_sound_update,
        .UpdateSoundParams = pico_sound_update_params,
        .StartSound = pico_sound_start,
        .StopSound = pico_sound_stop,
        .SoundIsPlaying = pico_sound_playing,
        .CacheSounds = pico_sound_cache,
};

static boolean pico_music_init(void)
{
        return false;
}

static void pico_music_shutdown(void)
{
}

static void pico_music_set_volume(int volume)
{
        (void)volume;
}

static void pico_music_pause(void)
{
}

static void pico_music_resume(void)
{
}

static void *pico_music_register(should_be_const void *data, int len)
{
        (void)data;
        (void)len;
        return NULL;
}

static void pico_music_unregister(void *handle)
{
        (void)handle;
}

static void pico_music_play(void *handle, boolean looping)
{
        (void)handle;
        (void)looping;
}

static void pico_music_stop(void)
{
}

static boolean pico_music_playing(void)
{
        return false;
}

static void pico_music_poll(void)
{
}

static const snddevice_t pico_music_devices[] = { SNDDEVICE_NONE };

const music_module_t music_opl_module = {
        .sound_devices = pico_music_devices,
        .num_sound_devices = (int)arrlen(pico_music_devices),
        .Init = pico_music_init,
        .Shutdown = pico_music_shutdown,
        .SetMusicVolume = pico_music_set_volume,
        .PauseMusic = pico_music_pause,
        .ResumeMusic = pico_music_resume,
        .RegisterSong = pico_music_register,
        .UnRegisterSong = pico_music_unregister,
        .PlaySong = pico_music_play,
        .StopSong = pico_music_stop,
        .MusicIsPlaying = pico_music_playing,
        .Poll = pico_music_poll,
};

