#ifndef LIBRETRO_COMMON_API_H
#define LIBRETRO_COMMON_API_H

#ifdef __cplusplus
#define RETRO_BEGIN_DECLS extern "C" {
#define RETRO_END_DECLS }
#else
#define RETRO_BEGIN_DECLS
#define RETRO_END_DECLS
#endif

#if defined(_WIN32) && !defined(_XBOX)
#define RETRO_CALLCONV __cdecl
#else
#define RETRO_CALLCONV
#endif

#if defined(_WIN32) && !defined(LIBRETRO_STATIC)
#if defined(LIBRETRO_CORE)
#define RETRO_API __declspec(dllexport)
#else
#define RETRO_API __declspec(dllimport)
#endif
#else
#define RETRO_API
#endif

#endif /* LIBRETRO_COMMON_API_H */
