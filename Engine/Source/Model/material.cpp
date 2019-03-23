#include "material.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "shader_parser.h"
#include "texture.h"

namespace Ming3D
{
    Material::Material(ParsedShaderProgram* shaderProgram)
    {
        size_t numTextures = shaderProgram->mShaderTextures.size();
        mShaderProgramData = shaderProgram;
        mMaterialBuffer = new MaterialBuffer();
        mTextures.resize(numTextures);
        mMaterialBuffer->mTextureBuffers.resize(numTextures);

        for (size_t iTexture = 0; iTexture < numTextures; iTexture++)
        {
            mTextures[iTexture] = nullptr;
            mMaterialBuffer->mTextureBuffers[iTexture] = nullptr;
        }

        // TODO: Queue render thread command
        mMaterialBuffer->mShaderProgram = GGameEngine->GetRenderDevice()->CreateShaderProgram(shaderProgram);
    }

    Material::~Material()
    {
        delete mShaderProgramData; // TODO: reference count
    }

    void Material::SetTexture(size_t textureIndex, Texture* texture)
    {
        if (mTextures[textureIndex] != nullptr)
            delete mTextures[textureIndex];

        mTextures[textureIndex] = texture;
        
        // TODO: Queue render thread command
        if (mMaterialBuffer->mTextureBuffers[textureIndex] != nullptr)
            delete mMaterialBuffer->mTextureBuffers[textureIndex];
        mMaterialBuffer->mTextureBuffers[textureIndex] = GGameEngine->GetRenderDevice()->CreateTextureBuffer(texture->GetTextureInfo(), texture->GetTextureData());
    }
}
