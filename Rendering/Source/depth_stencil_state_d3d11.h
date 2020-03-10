#ifndef MING3D_DEPTHSTENCILSTATED3D11_H
#define MING3D_DEPTHSTENCILSTATED3D11_H

#include "depth_stencil_state.h"

#include <d3d11.h>

namespace Ming3D::Rendering
{
    class DepthStencilStateD3D11 : public DepthStencilState
    {
        friend class RenderDeviceD3D11;
    private:
        ID3D11DepthStencilState* mDepthStencilState;
        D3D11_DEPTH_STENCIL_DESC mDepthStencilStateDesc;
    };
}

#endif
