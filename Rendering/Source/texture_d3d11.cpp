#include "texture_d3d11.h"

#include "render_device_d3d11.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    void TextureD3D11::BufferTexture()
    {
        __AssertComment(mBytesPerPixel % 2 == 0, "Bytes per pixel must be a power of 2");

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
        GRenderDeviceD3D11->GetDevice()->CreateShaderResourceView(boxTex, NULL, &mTextureResourceView);
    }
}
