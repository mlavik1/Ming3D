#ifndef MING3D_TEXTUREBUFFER_D3D11_H
#define MING3D_TEXTUREBUFFER_D3D11_H

#include "texture_buffer.h"

#include <d3d11.h>

namespace Ming3D
{
    class TextureBufferD3D11 : public TextureBuffer
    {
        friend class RenderDeviceD3D11;
    private:
        char* mData = nullptr;

    public:
        virtual ~TextureBufferD3D11();

        ID3D11Texture2D* mTexture = nullptr;
        ID3D11ShaderResourceView* mTextureResourceView = nullptr;

    };
}

#endif
