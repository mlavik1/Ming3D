#include "shader_writer.h"

namespace Ming3D { namespace ShaderConverter
{
    void ShaderStream::ClearStream()
    {
        mStream.clear();
    }

    void ShaderStream::AddIndent(int inCount)
    {
        mIndentCount += inCount;
    }

    void ShaderStream::RemoveIndent(int inCount)
    {
        mIndentCount -= inCount;
    }

    std::stringstream& ShaderStream::GetStream()
    {
        return mStream;
    }
}}
