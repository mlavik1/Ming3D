#include "platform_file_win32.h"

#include <Windows.h>

namespace Ming3D
{
    bool PlatformFileWin32::MakeDirectory(const std::string inPath)
    {
        return (CreateDirectoryA(inPath.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError());
    }

    bool PlatformFileWin32::DirectoryExists(const std::string inPath)
    {
        DWORD ftyp = GetFileAttributesA(inPath.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES)
            return false;  // bad path

        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
            return true;

        return false;
    }
}
