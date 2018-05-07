#include "texture.h"

namespace Ming3D
{
    void Texture::SetTextureData(const void* inData, const size_t inBytesPerPixel, PixelFormat inPixelFormat, unsigned int inWidth, unsigned int inHeight)
    {
        mWidth = inWidth;
        mHeight = inHeight;
        mBytesPerPixel = inBytesPerPixel;
        mPixelFormat = inPixelFormat;
        const size_t size = inBytesPerPixel * inWidth * inHeight;
        mTextureData.resize(size);
        memcpy(mTextureData.data(), inData, size);
    }
}
