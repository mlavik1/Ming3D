#ifndef MING3D_RASTERISERSTATED3D11_H
#define MING3D_RASTERISERSTATED3D11_H

#include "rasteriser_state.h"
#include <d3dx11.h>

namespace Ming3D
{
    class RasteriserStateD3D11 : public RasteriserState
    {
        friend class RenderDeviceD3D11;
    private:
        ID3D11RasterizerState* mRasteriserState;
        D3D11_RASTERIZER_DESC mRasteriserStateDesc;
    };
}

#endif
