#ifndef MING3D_TEXTURE_H
#define MING3D_TEXTURE_H

#include <vector> // TODO: Use a smart pointer to a stream

namespace Ming3D
{
    enum PixelFormat
    {
        RGBA, BGRA, RGB
    };

    class Texture
    {
    public:
        std::vector<char> mTextureData; // TODO: Use a smart pointer to a stream
        unsigned int mWidth = 0;
        unsigned int mHeight = 0;
        unsigned int mBytesPerPixel;
        PixelFormat mPixelFormat;

    public:
        virtual ~Texture() {}
        virtual void SetTextureData(const void* inData, const size_t inBytesPerPixel, PixelFormat inPixelFormat, unsigned int inWidth, unsigned int inHeight); // TODO: Use a smart pointer to a stream
        virtual void BufferTexture() = 0;
    };
}

#endif
