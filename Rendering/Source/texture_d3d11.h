#ifndef MING3D_TEXTURE_D3D11_H
#define MING3D_TEXTURE_D3D11_H

#include "texture.h"

#include <D3DX11.h>

namespace Ming3D
{
    class TextureD3D11 : public Texture
    {
    public:
        ID3D11ShaderResourceView* mTextureResourceView = nullptr;

    public:
        virtual void BufferTexture() override;

    };
}

#endif
