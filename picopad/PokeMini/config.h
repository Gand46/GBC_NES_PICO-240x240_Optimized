// Basic configuration for PokeMini port
#ifndef _CONFIG_H
#define _CONFIG_H

#define USE_ST7789 1
#define WIDTH 240
#define HEIGHT 240
#define USE_FRAMEBUF 0
#define FONT FontBold8x16
#define FONTW 8
#define FONTH 16

// Enable FAT filesystem support so platform library provides FileExist and
// related file utilities used by the PokeMini core
#define USE_FAT 1

#include CONFIG_DEF_H

#endif
