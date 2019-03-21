#ifndef MING3D_SHADER_PROGRAM_H
#define MING3D_SHADER_PROGRAM_H

#include "graphics_data.h"
#include <string.h>
#include "shader_info.h"

namespace Ming3D
{
    class ShaderUniformInfo
    {
    public:
        ShaderDatatypeInfo mType;
        std::string mName;
    
        ShaderUniformInfo(const ShaderDatatypeInfo& inType, const std::string& inName)
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
