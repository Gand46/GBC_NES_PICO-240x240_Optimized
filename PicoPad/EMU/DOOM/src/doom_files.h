// === DOOM filesystem helpers ===============================================
// Utilities to mount the storage device, prepare the directory layout used by
// the port and expose resolved paths for the engine and UI.

#pragma once

#include <cstddef>

struct DoomFileSystemStatus {
        bool storageMounted = false;
        bool wadFound = false;
        char basePath[APPPATH_PATHMAX + 1] = {0};
        char savePath[APPPATH_PATHMAX + 1] = {0};
        char wadPath[APPPATH_PATHMAX + 32] = {0};
};

// Mounts the storage device, ensures the DOOM directories exist and probes the
// expected WAD files. Returns true when a valid WAD was found.
bool DoomFilesInit();

// Returns the cached status produced by the last call to DoomFilesInit.
const DoomFileSystemStatus& DoomFilesGetStatus();

// Builds an absolute path rooted at the DOOM application directory. Accepts
// either an absolute path (which is copied verbatim) or a relative leaf.
// Returns false when the destination buffer is too small.
bool DoomFilesBuildPath(char* outPath, std::size_t outSize, const char* leafName);

