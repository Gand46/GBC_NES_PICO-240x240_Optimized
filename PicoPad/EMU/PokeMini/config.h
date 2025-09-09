#ifndef _CONFIG_H
#define _CONFIG_H

// Disable unused display outputs
#define USE_DISPHSTXMINI 0
#define USE_DISPHSTX 0
#define DISPHSTX_USE_DVI 0
#define DISPHSTX_USE_VGA 0

// Built-in ST7789 LCD
#define USE_ST7789 1
#define WIDTH 240
#define HEIGHT 240

// Enable basic text output
#define USE_DRAW_STDIO 1

#include CONFIG_DEF_H

#endif // _CONFIG_H
