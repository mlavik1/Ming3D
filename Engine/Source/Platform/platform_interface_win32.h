#pragma once

#include "platform_interface.h"

namespace Ming3D
{
    class PlatformInterfaceWin32 : public PlatformInterface
    {
    public:
        virtual bool MakeDirectory(const std::string& inPath) override;
        virtual bool DirectoryExists(const std::string& inPath) override;
        virtual bool OpenFileDialog(const std::string& inTitle, std::string& outFilePath) override;
    };
}
