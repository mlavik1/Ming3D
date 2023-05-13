#pragma once

#include <string>

namespace Ming3D
{
    class PlatformInterface
    {
    public:
        virtual ~PlatformInterface() = default;
        virtual bool MakeDirectory(const std::string& inPath) = 0;
        virtual bool DirectoryExists(const std::string& inPath) = 0;
        virtual bool OpenFileDialog(const std::string& inTitile, std::string& outFilePath) = 0;
    };
}
