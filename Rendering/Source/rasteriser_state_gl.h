#ifndef MING3D_RASTERISERSTATEGL_H
#define MING3D_RASTERISERSTATEGL_H

#include "rasteriser_state.h"

namespace Ming3D
{
    class RasteriserStateGL : public RasteriserState
    {
        friend class RenderDeviceGL;

    private:
        bool mDepthClipEnabled = true;
        RasteriserStateCullMode mCullMode = RasteriserStateCullMode::Front;
    };
}

#endif
