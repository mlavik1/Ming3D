#ifndef MING3D_DEPTHSTENCILSTATED3D11_H
#define MING3D_DEPTHSTENCILSTATED3D11_H

#include "depth_stencil_state.h"

#include <D3D11.h>

namespace Ming3D
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
