#ifdef MING3D_D3D11
#include "blend_state_d3d11.h"

namespace Ming3D::Rendering
{
    BlendStateD3D11::BlendStateD3D11(bool enableBlend, EBlendMode blendMode)
        : BlendState(enableBlend, blendMode)
    {
        ZeroMemory(&mBlendDesc, sizeof(D3D11_BLEND_DESC));
        mBlendDesc.RenderTarget[0].BlendEnable = enableBlend ? TRUE : FALSE;
        switch (blendMode)
        {
        case EBlendMode::Zero:
        {
            mBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
            break;
        }
        case EBlendMode::One:
        {
            mBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            break;
        }
        case EBlendMode::OneMinusSrcAlpha:
        {
            mBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            break;
        }
        }
        mBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        mBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        mBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        mBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        mBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        mBlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
    }

    BlendStateD3D11::~BlendStateD3D11()
    {

    }
}
#endif
