#include "texture_buffer_d3d11.h"

#include "render_device_d3d11.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    TextureBufferD3D11::~TextureBufferD3D11()
    {
        if (mTextureResourceView != nullptr)
        {
            mTextureResourceView->Release();
            delete mTextureResourceView;
        }
    }
}
