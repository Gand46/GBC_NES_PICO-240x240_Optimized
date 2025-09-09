#include "../streams/file_stream.h"
#include <stdio.h>
#include <stdlib.h>

struct RFILE {
    FILE *fp;
};

RFILE *filestream_open(const char *path, unsigned mode, unsigned hints)
{
    (void)hints;
    const char *m = "rb";
    if (mode & RETRO_VFS_FILE_ACCESS_WRITE)
        m = (mode & RETRO_VFS_FILE_ACCESS_READ) ? "rb+" : "wb";

    FILE *f = fopen(path, m);
    if (!f)
        return NULL;

    RFILE *r = (RFILE *)malloc(sizeof(*r));
    if (!r) {
        fclose(f);
        return NULL;
    }
    r->fp = f;
    return r;
}

void filestream_close(RFILE *stream)
{
    if (!stream)
        return;
    fclose(stream->fp);
    free(stream);
}

size_t filestream_read(RFILE *stream, void *data, size_t bytes)
{
    if (!stream)
        return 0;
    return fread(data, 1, bytes, stream->fp);
}

size_t filestream_write(RFILE *stream, const void *data, size_t bytes)
{
    if (!stream)
        return 0;
    return fwrite(data, 1, bytes, stream->fp);
}
