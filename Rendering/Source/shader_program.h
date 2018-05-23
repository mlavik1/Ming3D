#ifndef MING3D_SHADER_PROGRAM_H
#define MING3D_SHADER_PROGRAM_H

#include "graphics_data.h"
#include <string.h>

namespace Ming3D
{
    enum ShaderVariableType
    {
        Float,
        Int,
        Mat4x4,
        Vec4
    };

    class ShaderUniformInfo
    {
    public:
        ShaderVariableType mType;
        std::string mName;
    
        ShaderUniformInfo(const ShaderVariableType& inType, const std::string& inName)
        {
            mType = inType;
            mName = inName;
        }
    };

    struct ShaderProgramConstructionInfo
    {
        std::vector<ShaderUniformInfo> mUniforms;
        VertexLayout mVertexLayout;
    };

    class ShaderProgram
    {
    private:
        
    public:
        virtual ~ShaderProgram() {}
    };
}

#endif
