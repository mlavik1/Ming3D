#ifndef MING3D_DEPTHSTENCILSTATED3D11_H
#define MING3D_DEPTHSTENCILSTATED3D11_H

#include "depth_stencil_state.h"

#include <d3d11.h>

namespace Ming3D
{
    class DepthStencilStateD3D11 : public DepthStencilState
    {
        friend class RenderDeviceD3D11;
    private:
        ID3D11DepthStencilState* mDepthStencilState;
        ID3D11DepthStencilView* mDepthStencilView;
        D3D11_DEPTH_STENCIL_DESC mDepthStencilStateDesc;
        ID3D11Texture2D* mDepthStencilTexture;
    };
}

#endif
