#ifdef _WIN32
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

    bool PlatformFileWin32::OpenFileDialog(const std::string inTitile, std::string& outFilePath)
    {
        char filename[MAX_PATH];
        ZeroMemory(&filename, sizeof(filename));

        OPENFILENAMEA ofn;
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL; // TODO: get window handle
        ofn.lpstrFilter = "Any File\0*.*\0";
        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrTitle = inTitile.c_str();
        ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn))
        {
            size_t iChar = 0;
            while (filename[iChar] != 0)
            {
                if (filename[iChar] == '\\')
                    filename[iChar] = '/';
                iChar++;
            }
            outFilePath = filename;
            return true;
        }
        else
            return false;
    }
}
#endif
