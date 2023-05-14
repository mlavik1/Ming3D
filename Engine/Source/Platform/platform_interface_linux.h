#pragma once

#include "platform_interface.h"

namespace Ming3D
{
    class PlatformInterfaceLinux : public PlatformInterface
    {
    public:
        PlatformInterfaceLinux();
        virtual bool MakeDirectory(const std::string& inPath) override;
        virtual bool DirectoryExists(const std::string& inPath) override;
        virtual bool OpenFileDialog(const std::string& inTitle, std::string& outFilePath) override;

    private:
        bool hasKDialog = false;
        bool hasZenity = false;

        bool RunCommand(const std::string& command, std::string& outResult);
    };
}
