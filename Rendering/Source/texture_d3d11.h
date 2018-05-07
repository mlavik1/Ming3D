#ifndef MING3D_TEXTURE_D3D11_H
#define MING3D_TEXTURE_D3D11_H

#include "texture.h"

namespace Ming3D
{
    class TextureD3D11 : public Texture
    {
    public:
        virtual void BufferTexture() override;
    };
}

#endif
