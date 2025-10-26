#include "../include.h"

#include "doom_video.h"

extern "C" {
#include "engine/chocolate/i_video.h"
}

#include <algorithm>
#include <cstdint>
#include <cstring>

namespace
{

constexpr int kInternalWidth = DOOM_INTERNAL_WIDTH;
constexpr int kInternalHeight = DOOM_INTERNAL_HEIGHT;
constexpr int kDisplayWidth = DOOM_DISPLAY_WIDTH;
constexpr int kDisplayHeight = DOOM_DISPLAY_HEIGHT;

pixel_t software_framebuffer[kInternalWidth * kInternalHeight];
uint16_t palette_565[256];
uint8_t palette_rgb[256][3];
uint16_t display_framebuffer[kDisplayWidth * kDisplayHeight];
DoomVideoLayout layout = {0, 0, kDisplayWidth, kDisplayHeight};
int scale_factor_num = 1;
int scale_factor_den = 1;
uint16_t border_color = 0;
bool layout_computed = false;

inline uint16_t ConvertTo565(uint8_t r, uint8_t g, uint8_t b)
{
        return static_cast<uint16_t>(((r & 0xF8u) << 8) |
                                     ((g & 0xFCu) << 3) |
                                     (b >> 3));
}

inline int MapCoordinate(int dest)
{
        return (dest * scale_factor_den + scale_factor_num / 2) / scale_factor_num;
}

void ComputeLayout()
{
        const int width_num = kDisplayWidth;
        const int width_den = kInternalWidth;
        const int height_num = kDisplayHeight;
        const int height_den = kInternalHeight;

        if (width_num * height_den <= height_num * width_den)
        {
                scale_factor_num = width_num;
                scale_factor_den = width_den;
        }
        else
        {
                scale_factor_num = height_num;
                scale_factor_den = height_den;
        }

        layout.width = (kInternalWidth * scale_factor_num + scale_factor_den / 2) / scale_factor_den;
        layout.height = (kInternalHeight * scale_factor_num + scale_factor_den / 2) / scale_factor_den;

        if (layout.width > kDisplayWidth)
        {
                layout.width = kDisplayWidth;
        }

        if (layout.height > kDisplayHeight)
        {
                layout.height = kDisplayHeight;
        }

        layout.origin_x = (kDisplayWidth - layout.width) / 2;
        layout.origin_y = (kDisplayHeight - layout.height) / 2;

        layout_computed = true;
}

void ClearDisplay(uint16_t color)
{
        std::fill_n(display_framebuffer, kDisplayWidth * kDisplayHeight, color);
}

void BlitLetterbox()
{
        const int row_stride = kDisplayWidth;

        for (int y = 0; y < layout.origin_y; ++y)
        {
                std::fill_n(&display_framebuffer[y * row_stride], row_stride, border_color);
        }

        for (int dest_y = 0; dest_y < layout.height; ++dest_y)
        {
                const int src_y = MapCoordinate(dest_y);
                const pixel_t *src_row = &software_framebuffer[src_y * kInternalWidth];
                uint16_t *dst_row = &display_framebuffer[(layout.origin_y + dest_y) * row_stride];

                if (layout.origin_x > 0)
                {
                        std::fill_n(dst_row, layout.origin_x, border_color);
                }

                for (int dest_x = 0; dest_x < layout.width; ++dest_x)
                {
                        const int src_x = MapCoordinate(dest_x);
                        const pixel_t texel = src_row[src_x];
                        dst_row[layout.origin_x + dest_x] = palette_565[texel];
                }

                const int right_padding = row_stride - (layout.origin_x + layout.width);
                if (right_padding > 0)
                {
                        std::fill_n(dst_row + layout.origin_x + layout.width, right_padding, border_color);
                }
        }

        const int bottom_start = layout.origin_y + layout.height;
        for (int y = bottom_start; y < kDisplayHeight; ++y)
        {
                std::fill_n(&display_framebuffer[y * row_stride], row_stride, border_color);
        }
}

} // namespace

extern "C"
{

should_be_const constcharstar video_driver = "pico";
should_be_const constcharstar window_position = "center";
int video_display = 0;
int fullscreen = 1;
int aspect_ratio_correct = 0;
int integer_scaling = 1;
int vga_porch_flash = 0;
int window_width = DOOM_DISPLAY_WIDTH;
int window_height = DOOM_DISPLAY_HEIGHT;
int fullscreen_width = DOOM_DISPLAY_WIDTH;
int fullscreen_height = DOOM_DISPLAY_HEIGHT;
int force_software_renderer = 1;
int max_scaling_buffer_pixels = DOOM_DISPLAY_WIDTH * DOOM_DISPLAY_HEIGHT;
int startup_delay = 0;
int graphical_startup = 0;
int show_endoom = 0;
int show_diskicon = 0;
int png_screenshots = 0;
unsigned int joywait = 0;
isb_int8_t usegamma = 0;
boolean screenvisible = true;
boolean screensaver_mode = false;
pixel_t *I_VideoBuffer = NULL;
int screen_width = DOOM_INTERNAL_WIDTH;
int screen_height = DOOM_INTERNAL_HEIGHT;

void I_GraphicsCheckCommandLine(void)
{
}

void I_InitGraphics(void)
{
        if (!layout_computed)
        {
                ComputeLayout();
        }

        I_VideoBuffer = software_framebuffer;
        std::memset(I_VideoBuffer, 0, sizeof(software_framebuffer));

        border_color = palette_565[0];
        ClearDisplay(border_color);

        DrawImg(reinterpret_cast<const COLTYPE *>(display_framebuffer),
                0,
                0,
                DISP_OFFSET_X,
                DISP_OFFSET_Y,
                kDisplayWidth,
                kDisplayHeight,
                kDisplayWidth);
        DispUpdateAll();
}

void I_ShutdownGraphics(void)
{
        I_VideoBuffer = NULL;
}

void I_SetPalette(should_be_const byte *palette)
{
        if (palette == NULL)
        {
                return;
        }

        for (int i = 0; i < 256; ++i)
        {
                const uint8_t r = palette[i * 3 + 0];
                const uint8_t g = palette[i * 3 + 1];
                const uint8_t b = palette[i * 3 + 2];

                palette_rgb[i][0] = r;
                palette_rgb[i][1] = g;
                palette_rgb[i][2] = b;
                palette_565[i] = ConvertTo565(r, g, b);
        }

        border_color = palette_565[0];
}

int I_GetPaletteIndex(int r, int g, int b)
{
        for (int i = 0; i < 256; ++i)
        {
                if (palette_rgb[i][0] == r &&
                    palette_rgb[i][1] == g &&
                    palette_rgb[i][2] == b)
                {
                        return i;
                }
        }

        return 0;
}

void I_UpdateNoBlit(void)
{
}

void I_FinishUpdate(void)
{
        if (I_VideoBuffer == NULL)
        {
                return;
        }

        BlitLetterbox();

        DrawImg(reinterpret_cast<const COLTYPE *>(display_framebuffer),
                0,
                0,
                DISP_OFFSET_X,
                DISP_OFFSET_Y,
                kDisplayWidth,
                kDisplayHeight,
                kDisplayWidth);

        DispUpdate();
}

void I_ReadScreen(pixel_t *scr)
{
        if (scr != NULL && I_VideoBuffer != NULL)
        {
                std::memcpy(scr, I_VideoBuffer, sizeof(software_framebuffer));
        }
}

void I_BeginRead(void)
{
}

void I_SetWindowTitle(const char *title)
{
        (void)title;
}

void I_CheckIsScreensaver(void)
{
        screensaver_mode = false;
}

void I_SetGrabMouseCallback(grabmouse_callback_t func)
{
        (void)func;
}

void I_DisplayFPSDots(boolean dots_on)
{
        (void)dots_on;
}

void I_BindVideoVariables(void)
{
}

void I_InitWindowTitle(void)
{
}

void I_InitWindowIcon(void)
{
}

void I_StartFrame(void)
{
}

void I_EnableLoadingDisk(int xoffs, int yoffs)
{
        (void)xoffs;
        (void)yoffs;
}

} // extern "C"

const DoomVideoLayout *DoomVideoGetLayout(void)
{
        if (!layout_computed)
        {
                ComputeLayout();
        }

        return &layout;
}

