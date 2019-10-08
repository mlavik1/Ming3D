#ifndef MING3D_PLATFORMFILELINUX_H
#define MING3D_PLATFORMFILELINUX_H

#include "platform_file.h"

namespace Ming3D
{
    class PlatformFileLinux : public PlatformFile
    {
    public:
        virtual bool MakeDirectory(const std::string inPath) override;
        virtual bool DirectoryExists(const std::string inPath) override;
    };
}

#endif
