#ifndef MING3D_DEPTHSTENCILVIEWD3D11_H
#define MING3D_DEPTHSTENCILVIEWD3D11_H

#include <d3d11.h>
#include "texture_buffer_d3d11.h"

namespace Ming3D
{
    class DepthStencilViewD3D11
    {
    public:
        ~DepthStencilViewD3D11();

        TextureBufferD3D11* mDepthStencilTexture;
        ID3D11DepthStencilView* mDepthStencilView;
    };
}

#endif
