#ifndef MING3D_SHADERCONSTANTGL_H
#define MING3D_SHADERCONSTANTGL_H

#include "shader_constant.h"
#include "GL/glew.h"
#include <limits>

namespace Ming3D::Rendering
{
    class ShaderConstantGL : public ShaderConstant
    {
    public:
        GLuint mID = UINT_MAX;

        ShaderConstantGL(ShaderUniformInfo inInfo)
            : ShaderConstant(inInfo)
        {

        }

        bool IsLocationSet() { return mID != UINT_MAX; }
    };
}

#endif
