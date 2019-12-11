#ifdef __linux__
#include "platform_file_linux.h"


namespace Ming3D
{
    bool PlatformFileLinux::MakeDirectory(const std::string inPath)
    {
        return false;
    }

    bool PlatformFileLinux::DirectoryExists(const std::string inPath)
    {
        return false;
    }

    bool PlatformFileLinux::OpenFileDialog(const std::string inTitile, std::string& outFilePath)
    {
        return false;
    }
}
#endif
