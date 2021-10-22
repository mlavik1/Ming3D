#pragma once
#include "blend_state.h"
#include <d3d11.h>

namespace Ming3D::Rendering
{
    class BlendStateD3D11 : public BlendState
    {
    public:
        BlendStateD3D11(bool enableBlend, EBlendMode blendMode);
        virtual ~BlendStateD3D11();

        D3D11_BLEND_DESC mBlendDesc;
        ID3D11BlendState* mBlendState = nullptr;
        float mBlendFactor[4] = { 1, 1, 1, 1 };
    };
}
