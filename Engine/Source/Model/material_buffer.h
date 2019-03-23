#ifndef MING3D_MATERIALBUFFER_H
#define MING3D_MATERIALBUFFER_H

#include <vector>

namespace Ming3D
{
    // forward declarations
    class ShaderProgram;
    class TextureBuffer;

    class MaterialBuffer
    {
    public:
        ShaderProgram* mShaderProgram = nullptr;
        std::vector<TextureBuffer*> mTextureBuffers;
    };
}

#endif
