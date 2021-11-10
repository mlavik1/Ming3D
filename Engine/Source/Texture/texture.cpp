#include "texture.h"
#include <cstring>

namespace Ming3D
{
    uint64_t Texture::GUIDCounter = 0;

    Texture::Texture()
        : mGUID(GUIDCounter++)
    {
    }

    void Texture::SetTextureData(const void* inData, const size_t inBytesPerPixel, PixelFormat inPixelFormat, unsigned int inWidth, unsigned int inHeight)
    {
        mTextureInfo.mWidth = inWidth;
        mTextureInfo.mHeight = inHeight;
        mTextureInfo.mBytesPerPixel = inBytesPerPixel;
        mTextureInfo.mPixelFormat = inPixelFormat;
        const size_t size = inBytesPerPixel * inWidth * inHeight;
        mTextureData.resize(size);
        memcpy(mTextureData.data(), inData, size);
    }
}
