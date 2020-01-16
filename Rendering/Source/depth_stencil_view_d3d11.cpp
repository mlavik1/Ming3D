#include "depth_stencil_view_d3d11.h"

namespace Ming3D
{
    DepthStencilViewD3D11::~DepthStencilViewD3D11()
    {
        delete(mDepthStencilTexture);
        mDepthStencilView->Release();
    }
}
