#ifndef MING3D_PLATFORMFILE_H
#define MING3D_PLATFORMFILE_H

#include <string>

namespace Ming3D
{
    class PlatformFile
    {
    public:
        virtual bool MakeDirectory(const std::string inPath) = 0;
        virtual bool DirectoryExists(const std::string inPath) = 0;
    };
}

#endif
