#ifndef FILE_STREAM_H
#define FILE_STREAM_H
#include "retro_common_api.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct RFILE { int dummy; } RFILE;

static inline RFILE* filestream_open(const char *path, unsigned mode, unsigned hints)
{
    (void)path; (void)mode; (void)hints; return NULL;
}

static inline int64_t filestream_read(RFILE *stream, void *data, int64_t len)
{
    (void)stream; (void)data; (void)len; return 0;
}

static inline int64_t filestream_write(RFILE *stream, const void *data, int64_t len)
{
    (void)stream; (void)data; (void)len; return 0;
}

static inline int filestream_close(RFILE *stream)
{
    (void)stream; return 0;
}

#endif /* FILE_STREAM_H */
