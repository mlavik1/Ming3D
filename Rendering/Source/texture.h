#ifndef MING3D_TEXTURE_H
#define MING3D_TEXTURE_H

#include <vector> // TODO: Use a smart pointer to a stream
#include <cstring>

namespace Ming3D
{
    enum PixelFormat
    {
        RGBA, BGRA, RGB
    };

    struct TextureInfo
    {
        unsigned int mWidth = 0;
        unsigned int mHeight = 0;
        unsigned int mBytesPerPixel;
        PixelFormat mPixelFormat;
    };

    /**
    * Base class for  Textures.
    * There will exist a subclass for each rendering API.
    * Created by the RenderDevice.
    */
    class Texture
    {
    public:
        std::vector<char> mTextureData; // TODO: Use a smart pointer to a stream
        TextureInfo mTextureInfo;

    public:
        virtual ~Texture() {}
        void SetTextureData(const void* inData, const size_t inBytesPerPixel, PixelFormat inPixelFormat, unsigned int inWidth, unsigned int inHeight); // TODO: Use a smart pointer to a stream
        
        void* GetTextureData() { return mTextureData.data(); }
        TextureInfo GetTextureInfo() const { return mTextureInfo; }
    };
}

#endif
