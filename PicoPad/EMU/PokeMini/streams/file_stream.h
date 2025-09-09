#ifndef FILE_STREAM_H
#define FILE_STREAM_H

#include <stddef.h>
#include <retro_common_api.h>

#define RETRO_VFS_FILE_ACCESS_READ       1
#define RETRO_VFS_FILE_ACCESS_WRITE      2
#define RETRO_VFS_FILE_ACCESS_READ_WRITE (RETRO_VFS_FILE_ACCESS_READ|RETRO_VFS_FILE_ACCESS_WRITE)
#define RETRO_VFS_FILE_ACCESS_HINT_NONE  0

RETRO_BEGIN_DECLS

typedef struct RFILE RFILE;

RFILE *filestream_open(const char *path, unsigned mode, unsigned hints);
void filestream_close(RFILE *stream);
size_t filestream_read(RFILE *stream, void *data, size_t bytes);
size_t filestream_write(RFILE *stream, const void *data, size_t bytes);

RETRO_END_DECLS

#endif /* FILE_STREAM_H */
