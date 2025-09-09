#ifndef _LIBRETRO_SDK_FILE_MEMORY_STREAM_H
#define _LIBRETRO_SDK_FILE_MEMORY_STREAM_H

#include <stdint.h>
#include <stddef.h>
#include <retro_common_api.h>

RETRO_BEGIN_DECLS

typedef struct memstream memstream_t;

memstream_t *memstream_open(unsigned writing);
void memstream_close(memstream_t *stream);
uint64_t memstream_read(memstream_t *stream, void *data, uint64_t bytes);
uint64_t memstream_write(memstream_t *stream, const void *data, uint64_t bytes);
int memstream_getc(memstream_t *stream);
void memstream_putc(memstream_t *stream, int c);
char *memstream_gets(memstream_t *stream, char *s, size_t len);
uint64_t memstream_pos(memstream_t *stream);
void memstream_rewind(memstream_t *stream);
int64_t memstream_seek(memstream_t *stream, int64_t offset, int whence);
void memstream_set_buffer(uint8_t *buffer, uint64_t size);
uint64_t memstream_get_last_size(void);
uint64_t memstream_get_ptr(memstream_t *stream);

RETRO_END_DECLS

#endif /* _LIBRETRO_SDK_FILE_MEMORY_STREAM_H */
