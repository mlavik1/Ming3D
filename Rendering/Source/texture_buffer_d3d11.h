#ifndef MING3D_TEXTUREBUFFER_D3D11_H
#define MING3D_TEXTUREBUFFER_D3D11_H

#include "texture_buffer.h"

#include <D3DX11.h>

namespace Ming3D
{
    class TextureBufferD3D11 : public TextureBuffer
    {
    public:
        virtual ~TextureBufferD3D11();

        ID3D11ShaderResourceView* mTextureResourceView = nullptr;

    };
}

#endif
