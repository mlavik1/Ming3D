#ifndef MING3D_MATERIAL_H
#define MING3D_MATERIAL_H

#include "material_buffer.h"
#include "shader_parser.h"

namespace Ming3D
{
    class Material
    {
    public:
        MaterialBuffer* mMaterialBuffer;
        ShaderConverter::ParsedShaderProgram* mShaderProgramData;

        Material(ShaderConverter::ParsedShaderProgram* shaderProgram);
        ~Material();
    };
}

#endif
