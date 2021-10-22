#pragma once

namespace Ming3D::Rendering
{
    enum class EBlendMode
    {
        Zero, One, OneMinusSrcAlpha
    };

    class BlendState
    {
    public:
        BlendState(bool enableBlend, EBlendMode blendMode) {}
        virtual ~BlendState() {}
    };
}
