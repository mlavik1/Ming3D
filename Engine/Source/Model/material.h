#ifndef MING3D_MATERIAL_H
#define MING3D_MATERIAL_H

#include "material_buffer.h"
#include <vector>

namespace Ming3D
{
    class ParsedShaderProgram;
    class Texture;

    class Material
    {
    public:
        MaterialBuffer* mMaterialBuffer;
        ParsedShaderProgram* mShaderProgramData;
        std::vector<Texture*> mTextures;

        Material(ParsedShaderProgram* shaderProgram);
        ~Material();

        void SetTexture(size_t textureIndex, Texture* texture);
    };
}

#endif
