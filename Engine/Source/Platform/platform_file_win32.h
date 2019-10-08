#ifndef MING3D_PLATFORMFILEWIN32_H
#define MING3D_PLATFORMFILEWIN32_H

#include "platform_file.h"

namespace Ming3D
{
    class PlatformFileWin32 : public PlatformFile
    {
    public:
        virtual bool MakeDirectory(const std::string inPath) override;
        virtual bool DirectoryExists(const std::string inPath) override;
    };
}

#endif
