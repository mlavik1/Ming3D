#ifndef MING3D_RASTERISERSTATE_H
#define MING3D_RASTERISERSTATE_H

namespace Ming3D::Rendering
{
    enum class RasteriserStateCullMode
    {
        None, Front, Back
    };

    class RasteriserState
    {
    public:
        virtual ~RasteriserState() = default;
    };
}

#endif
