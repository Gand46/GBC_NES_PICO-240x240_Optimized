#include "../streams/file_stream.h"

struct RFILE { int dummy; };

RFILE *filestream_open(const char *path, unsigned mode, unsigned hints)
{
    (void)path; (void)mode; (void)hints;
    return NULL;
}

void filestream_close(RFILE *stream)
{
    (void)stream;
}

size_t filestream_read(RFILE *stream, void *data, size_t bytes)
{
    (void)stream; (void)data; (void)bytes;
    return 0;
}

size_t filestream_write(RFILE *stream, const void *data, size_t bytes)
{
    (void)stream; (void)data; (void)bytes;
    return 0;
}
