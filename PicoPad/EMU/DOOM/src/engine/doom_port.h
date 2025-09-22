#pragma once

#include "../setup.h"
#include "../../config.h"

#ifdef __cplusplus
extern "C" {
#endif

// === Environment feature flags =================================================
#ifndef PICO_BUILD
#define PICO_BUILD              1
#endif

#ifndef PICO_ON_DEVICE
#define PICO_ON_DEVICE          1
#endif

#ifndef LIB_PICO_STDLIB
#define LIB_PICO_STDLIB         1
#endif

#ifndef LIB_PICO_STDIO
#define LIB_PICO_STDIO          0
#endif

#ifndef USE_PICO_NET
#define USE_PICO_NET            0
#endif

#ifndef HAVE_MMAP
#define HAVE_MMAP               0
#endif

#ifndef HAVE_DIRENT_H
#define HAVE_DIRENT_H           0
#endif

#ifndef HAVE_DECL_STRCASECMP
#define HAVE_DECL_STRCASECMP    0
#endif

#ifndef HAVE_DECL_STRNCASECMP
#define HAVE_DECL_STRNCASECMP   0
#endif

#ifndef USE_WHD
#define USE_WHD                 0
#endif

#ifndef USE_FLAT_MAX_256
#define USE_FLAT_MAX_256        1
#endif

#ifndef NO_USE_ARGS
#define NO_USE_ARGS             1
#endif

#ifndef NO_USE_MOUSE
#define NO_USE_MOUSE            1
#endif

#ifndef NO_USE_JOYSTICK
#define NO_USE_JOYSTICK         1
#endif

#ifndef NO_USE_DEH
#define NO_USE_DEH              1
#endif

#ifndef NO_USE_RELOAD
#define NO_USE_RELOAD           1
#endif

#ifndef SOUND_LOW_PASS
#define SOUND_LOW_PASS          1
#endif

#ifndef NUM_SOUND_CHANNELS
#define NUM_SOUND_CHANNELS      8
#endif

// === Timing and geometry helpers ==============================================
#ifndef TICRATE
#define TICRATE                 DOOM_TICK_RATE
#endif

#ifndef SCREENWIDTH
#define SCREENWIDTH             DOOM_INTERNAL_WIDTH
#endif

#ifndef SCREENHEIGHT
#define SCREENHEIGHT            DOOM_INTERNAL_HEIGHT
#endif

#ifndef SCREENHEIGHT_4_3
#define SCREENHEIGHT_4_3        240
#endif

#ifdef __cplusplus
} // extern "C"
#endif

