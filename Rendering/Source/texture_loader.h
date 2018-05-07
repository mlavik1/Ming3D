#ifndef MING3D_TEXTURELOADER_H
#define MING3D_TEXTURELOADER_H

#include "texture.h"

namespace Ming3D
{
    class TextureLoader
    {
    public:
        static void LoadTextureData(const char* inFilePath, Texture* outTexture);
    };
}

#endif
