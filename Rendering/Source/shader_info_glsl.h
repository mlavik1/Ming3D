#ifndef MING3D_SHADERINFOGLSL_H
#define MING3D_SHADERINFOGLSL_H

#include "shader_info.h"
#include <string>

namespace Ming3D::Rendering
{
    struct ShaderDataGLSL
    {
        std::string mSource;
    };

    struct ShaderProgramDataGLSL
    {
        ShaderDataGLSL mVertexShader;
        ShaderDataGLSL mFragmentShader;
    };

    class ConvertedShaderProgramGLSL : public ConvertedShaderProgram
    {
    public:
        ShaderProgramDataGLSL mShaderProgramData;
    };
}

#endif
