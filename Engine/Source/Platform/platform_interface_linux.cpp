#ifdef __linux__
#include "platform_interface_linux.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>

namespace Ming3D
{
    PlatformInterfaceLinux::PlatformInterfaceLinux()
    {
        int result = system("kdialog");
        hasKDialog = result == 0;
        result = system("zenity");
        hasZenity = result == 0;
    }

    bool PlatformInterfaceLinux::MakeDirectory(const std::string& inPath)
    {
        return false;
    }

    bool PlatformInterfaceLinux::DirectoryExists(const std::string& inPath)
    {
        return false;
    }

    bool PlatformInterfaceLinux::OpenFileDialog(const std::string& inTitle, std::string& outFilePath)
    {
        if (hasKDialog)
        {
            return RunCommand("kdialog --getopenfilename --title=\"" + inTitle + "\"", outFilePath);
        }
        else if(hasZenity)
        {
            return RunCommand("zenity --file-selection --title=\"" + inTitle + "\"", outFilePath);
        }
        else
            return false;
    }

    bool PlatformInterfaceLinux::RunCommand(const std::string& command, std::string& outResult)
    {
        char buffer[128];
        std::string result = "";

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe)
            return false;

        while (!feof(pipe))
        {

            if (fgets(buffer, 128, pipe) != nullptr)
                result += buffer;
        }

        pclose(pipe);
        // Remove new line
        result[strcspn(buffer, "\r\n")] = 0;
        outResult = result;
        return true;
    }

}
#endif
