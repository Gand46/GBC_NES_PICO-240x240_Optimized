#include "../include.h"

#include "doom_files.h"

#include <cstddef>
#include <cstdio>
#include <cstring>

namespace {

constexpr const char* kWadCandidates[] = {
        "doom1.wad",
        "doomu.wad",
        "doom.wad",
        "doom2.wad",
        "freedoom1.wad",
        "freedoom2.wad",
        nullptr,
};

DoomFileSystemStatus gStatus;

bool CopyString(char* dst, std::size_t dstSize, const char* src)
{
        if (dst == nullptr || dstSize == 0)
                return false;

        if (src == nullptr)
        {
                dst[0] = '\0';
                return true;
        }

        int written = std::snprintf(dst, dstSize, "%s", src);
        if (written < 0)
                return false;

        if (static_cast<std::size_t>(written) >= dstSize)
        {
                dst[dstSize - 1] = '\0';
                return false;
        }

        return true;
}

bool EnsureDirectory(const char* path)
{
        if ((path == nullptr) || (path[0] == '\0'))
                return false;

        if (SetDir(path))
                return true;

        if (!DirCreate(path))
                return false;

        return SetDir(path);
}

const char* TrimPrefix(const char* absolutePath, const char* prefix)
{
        if (absolutePath == nullptr)
                return "";

        if ((absolutePath[0] != '/') && (absolutePath[0] != '\\'))
                return absolutePath;

        if (prefix == nullptr)
                return absolutePath;

        std::size_t prefixLen = std::strlen(prefix);
        if (prefixLen == 0)
                return absolutePath;

        if (std::strncmp(absolutePath, prefix, prefixLen) != 0)
                return absolutePath;

        const char* relative = absolutePath + prefixLen;
        while ((*relative == '/') || (*relative == '\\'))
                ++relative;

        return relative;
}

bool BuildPathInternal(char* outPath, std::size_t outSize, const char* basePath, const char* leafName)
{
        if ((outPath == nullptr) || (outSize == 0))
                return false;

        outPath[0] = '\0';

        if (leafName == nullptr)
                return false;

        // Absolute path: copy verbatim.
        if ((leafName[0] == '/') || (leafName[0] == '\\'))
        {
                int written = std::snprintf(outPath, outSize, "%s", leafName);
                return (written >= 0) && (static_cast<std::size_t>(written) < outSize);
        }

        const bool baseEmpty = (basePath == nullptr) || (basePath[0] == '\0');
        if (baseEmpty)
        {
                int written = std::snprintf(outPath, outSize, "%s", leafName);
                return (written >= 0) && (static_cast<std::size_t>(written) < outSize);
        }

        const char* base = basePath;
        std::size_t baseLen = std::strlen(base);
        bool needsSlash = (baseLen > 0) && (base[baseLen - 1] != '/') && (base[baseLen - 1] != '\\');

        int written = std::snprintf(outPath, outSize, needsSlash ? "%s/%s" : "%s%s", base, leafName);
        return (written >= 0) && (static_cast<std::size_t>(written) < outSize);
}

} // namespace

bool DoomFilesBuildPath(char* outPath, std::size_t outSize, const char* leafName)
{
        return BuildPathInternal(outPath, outSize, gStatus.basePath, leafName);
}

const DoomFileSystemStatus& DoomFilesGetStatus()
{
        return gStatus;
}

bool DoomFilesInit()
{
        // Reset cached status before probing.
        gStatus = DoomFileSystemStatus{};

        if (!DiskAutoMount())
                return false;

        gStatus.storageMounted = true;

        char resolvedBase[APPPATH_PATHMAX + 1] = {0};
        int baseLen = GetHomePath(resolvedBase, DOOM_APP_FOLDER);
        if ((baseLen <= 0) || (baseLen > APPPATH_PATHMAX))
        {
                // Fallback to the default macro when metadata is unavailable.
                CopyString(resolvedBase, sizeof(resolvedBase), DOOM_APP_FOLDER);
        }

        CopyString(gStatus.basePath, sizeof(gStatus.basePath), resolvedBase);

        if (!EnsureDirectory(gStatus.basePath))
                return false;

        // Restore current directory to the application root before continuing.
        SetDir(gStatus.basePath);

        const char* saveMacro = TrimPrefix(DOOM_SAVE_FOLDER, gStatus.basePath);
        if (!DoomFilesBuildPath(gStatus.savePath, sizeof(gStatus.savePath), saveMacro))
                return false;

        // Prepare the save folder (ignore errors if it already exists).
        if (!EnsureDirectory(gStatus.savePath))
                return false;

        // Return to the base folder after touching the save directory.
        SetDir(gStatus.basePath);

        char candidatePath[APPPATH_PATHMAX + 32] = {0};
        for (const char* candidate : kWadCandidates)
        {
                if (candidate == nullptr)
                        break;

                if (!DoomFilesBuildPath(candidatePath, sizeof(candidatePath), candidate))
                        continue;

                if (FileExist(candidatePath))
                {
                        CopyString(gStatus.wadPath, sizeof(gStatus.wadPath), candidatePath);
                        gStatus.wadFound = true;
                        break;
                }
        }

        return gStatus.wadFound;
}

