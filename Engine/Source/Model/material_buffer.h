#ifndef MING3D_MATERIALBUFFER_H
#define MING3D_MATERIALBUFFER_H

#include "shader_program.h"
#include "texture_buffer.h"
#include <vector>

namespace Ming3D
{
    class MaterialBuffer
    {
    public:
        ShaderProgram* mShaderProgram = nullptr;
        std::vector<TextureBuffer*> mTextureBuffers;
    };
}

#endif
