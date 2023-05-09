#ifndef MING3D_TEXTUREINFO_H
#define MING3D_TEXTUREINFO_H

namespace Ming3D
{
    enum class PixelFormat
    {
        RGBA, BGRA, RGB
    };

    enum class TextureWrapMode
    {
        Repeat,
        Clamp
    };

    struct TextureInfo
    {
        unsigned int mWidth = 0;
        unsigned int mHeight = 0;
        unsigned int mBytesPerPixel = 0;
        PixelFormat mPixelFormat = PixelFormat::RGBA;
        TextureWrapMode mTextureWrapMode = TextureWrapMode::Repeat;
    };
}

#endif
