#ifndef LIBRETRO_H__
#define LIBRETRO_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Minimal libretro definitions required for standalone build */
typedef void (*retro_log_printf_t)(int level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* LIBRETRO_H__ */
