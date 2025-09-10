// ****************************************************************************
//
//                        Project library configuration
//
// ****************************************************************************

#ifndef _CONFIG_H
#define _CONFIG_H

// Disable high-speed digital display outputs
#define USE_DISPHSTXMINI    0
#define USE_DISPHSTX        0
#define DISPHSTX_USE_DVI    0
#define DISPHSTX_USE_VGA    0

// Configure built-in ST7789 LCD
#define USE_ST7789          1
#define WIDTH               240
#define HEIGHT              240

// Frame buffer and font settings
#define USE_FRAMEBUF        0
#define FONT                FontBold8x16
#define FONTW               8
#define FONTH               16

// Enable basic PWM sound output
#define USE_PWMSND          2
#define PWMSND_GPIO         15
#define PWMSND_GPIO_R       14

// Enable USB keyboard for input handling
#define USE_USB_HOST_HID    4

#include CONFIG_DEF_H

#endif // _CONFIG_H
