#ifndef MING3D_TEXTURELOADER_H
#define MING3D_TEXTURELOADER_H

#include "texture.h"
#include <string>

namespace Ming3D
{
    class TextureLoader
    {
    public:
        static Texture* LoadTextureData(const std::string inFilePath);
        static void CreateEmptyTexture(Texture* outTexture);
    };
}

#endif
