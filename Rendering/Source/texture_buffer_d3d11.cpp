#ifdef MING3D_D3D11
#include "texture_buffer_d3d11.h"

#include "render_device_d3d11.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    TextureBufferD3D11::~TextureBufferD3D11()
    {
        if (mTexture != nullptr)
        {
            mTexture->Release();
        }
        if (mTextureResourceView != nullptr)
        {
            mTextureResourceView->Release();
            delete mTextureResourceView;
        }
        if (mData != nullptr)
        {
            delete[] mData;
        }
    }
}
#endif

