#ifndef MING3D_TEXTURE_H
#define MING3D_TEXTURE_H

#include <vector> // TODO: Use a smart pointer to a stream
#include <cstring>
#include "texture_info.h"
#include <cstdint>

namespace Ming3D
{
    /**
    * Base class for  Textures.
    * There will exist a subclass for each rendering API.
    * Created by the RenderDevice.
    */
    class Texture
    {
    private:
        static uint64_t GUIDCounter;

    public:
        std::vector<char> mTextureData; // TODO: Use a smart pointer to a stream
        TextureInfo mTextureInfo;
        const uint64_t mGUID;

    public:
        Texture();
        virtual ~Texture() {}
        void SetTextureData(const void* inData, const size_t inBytesPerPixel, PixelFormat inPixelFormat, unsigned int inWidth, unsigned int inHeight); // TODO: Use a smart pointer to a stream
        
        void* GetTextureData() { return mTextureData.data(); }
        TextureInfo GetTextureInfo() const { return mTextureInfo; }
    };
}

#endif
