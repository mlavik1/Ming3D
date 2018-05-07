#include "texture_d3d11.h"

#include <D3DX11.h>

#include "render_device_d3d11.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    void TextureD3D11::BufferTexture()
    {
        __AssertComment(mBytesPerPixel % 2 == 0, "Bytes per pixel must be a power of 2");

        ID3D11ShaderResourceView* CubesTexture;
        ID3D11SamplerState* CubesTexSamplerState;

        const unsigned int w = mWidth;
        const unsigned int h = mHeight;

        ID3D11Texture2D *boxTex = 0;

        void* dataPtr = mTextureData.data();

        DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        switch (mPixelFormat)
        {
            case PixelFormat::RGB:
            {
                if (mBytesPerPixel == 12)
                {
                    dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
                }
                else
                {
                    __AssertComment(0, "Unhandled pixel format");
                }
                break;
            }
            case PixelFormat::RGBA:
            {
                if (mBytesPerPixel == 4)
                {
                    dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
                }
                else if (mBytesPerPixel == 8)
                {
                    dxgiFormat = DXGI_FORMAT_R16G16B16A16_UNORM;

                }
                else if (mBytesPerPixel == 16)
                {
                    dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

                }
                else
                {
                    __AssertComment(0, "Unhandled pixel format");
                }
                break;
            }
            case PixelFormat::BGRA:
            {
                if (mBytesPerPixel == 4)
                {
                    dxgiFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
                }
                else
                {
                    __AssertComment(0, "Unhandled pixel format");
                }
                break;
            }
        }

        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = dataPtr;
        initData.SysMemPitch = w * 4 * sizeof(uint8_t);
        initData.SysMemSlicePitch = w * h * 4 * sizeof(uint8_t);

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = w;
        desc.Height = h;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = dxgiFormat;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        GRenderDeviceD3D11->GetDevice()->CreateTexture2D(&desc, &initData, &boxTex);
        GRenderDeviceD3D11->GetDevice()->CreateShaderResourceView(boxTex, NULL, &CubesTexture);


        D3D11_SAMPLER_DESC sampDesc;
        ZeroMemory(&sampDesc, sizeof(sampDesc));
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
        HRESULT hr = GRenderDeviceD3D11->GetDevice()->CreateSamplerState(&sampDesc, &CubesTexSamplerState);


        GRenderDeviceD3D11->GetDeviceContext()->PSSetShaderResources(0, 1, &CubesTexture);
        GRenderDeviceD3D11->GetDeviceContext()->PSSetSamplers(0, 1, &CubesTexSamplerState);
    }
}
