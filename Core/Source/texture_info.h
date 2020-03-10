#ifndef MING3D_TEXTUREINFO_H
#define MING3D_TEXTUREINFO_H

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
}

#endif
