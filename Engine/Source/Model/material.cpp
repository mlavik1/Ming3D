#include "material.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "shader_parser.h"
#include "Texture/texture.h"
#include "shader_info.h"
#include "shader_uniform_data.h"
#include "Debug/st_assert.h"
#include "SceneRenderer/scene_renderer.h" // TODO

namespace Ming3D
{
    Material::Material(Rendering::ParsedShaderProgram* shaderProgram)
    {
        size_t numTextures = shaderProgram->mShaderTextures.size();
        mMaterialBuffer = new MaterialBuffer();
        mTextures.resize(numTextures);
        mMaterialBuffer->mTextureBuffers.resize(numTextures);

        for (size_t iTexture = 0; iTexture < numTextures; iTexture++)
        {
            mTextures[iTexture] = nullptr;
            mMaterialBuffer->mTextureBuffers[iTexture] = nullptr;
            mMaterialBuffer->mTextureIDs[shaderProgram->mShaderTextures[iTexture].mTextureName] = iTexture;
        }

        int zeroValues[128] = { }; // zero-initialised

        for (size_t iCB = 0; iCB < shaderProgram->mConstantBufferInfos.size(); iCB++)
        {
            const Rendering::ConstantBufferInfo& constantBuffer = shaderProgram->mConstantBufferInfos[iCB];
            mMaterialBuffer->mConstantBuffers.insert(constantBuffer.mName);
        }

        for (size_t iUniform = 0; iUniform < shaderProgram->mUniforms.size(); iUniform++)
        {
            const Rendering::ShaderVariableInfo& uniform = shaderProgram->mUniforms[iUniform];
            ShaderUniformData* uniformData = new ShaderUniformData(uniform.mDatatypeInfo, uniform.mDatatypeInfo.GetDataSize());
            switch (uniform.mDatatypeInfo.mDatatype)
            {
            case Rendering::EShaderDatatype::Mat4x4:
            {
                glm::mat4 identMat(1.0f);
                uniformData->SetData(&identMat);
                break;
            }
            default:
                uniformData->SetData(&zeroValues);
            }
            mMaterialBuffer->mShaderUniformMap.emplace(uniform.mName, uniformData);
        }

        // TODO: Queue render thread command
        mMaterialBuffer->mShaderProgram = GGameEngine->GetRenderDevice()->CreateShaderProgram(shaderProgram);
        GGameEngine->GetSceneRenderer()->RegisterMaterial(mMaterialBuffer); // TODO
    }

    Material::~Material()
    {
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

    void Material::SetTexture(const std::string& textureName, Texture* texture)
    {
        SetTexture(mMaterialBuffer->GetTextureID(textureName), texture);
    }

    void Material::SetShaderUniformFloat(const std::string& inName, float inVal)
    {
        mMaterialBuffer->SetShaderUniformFloat(inName, inVal);
    }

    void Material::SetShaderUniformInt(const std::string& inName, int inVal)
    {
        mMaterialBuffer->SetShaderUniformInt(inName, inVal);
    }

    void Material::SetShaderUniformVec2(const std::string& inName, const glm::vec2& inVal)
    {
        mMaterialBuffer->SetShaderUniformVec2(inName, inVal);
    }

    void Material::SetShaderUniformVec3(const std::string& inName, const glm::vec3& inVal)
    {
        mMaterialBuffer->SetShaderUniformVec3(inName, inVal);
    }

    void Material::SetShaderUniformVec4(const std::string& inName, const glm::vec4& inVal)
    {
        mMaterialBuffer->SetShaderUniformVec4(inName, inVal);
    }

    void Material::SetShaderUniformMat4x4(const std::string& inName, const glm::mat4& inVal)
    {
        mMaterialBuffer->SetShaderUniformMat4x4(inName, inVal);
    }

    bool Material::HasShaderUniform(const std::string& inName)
    {
        return mMaterialBuffer->mShaderUniformMap.find(inName) != mMaterialBuffer->mShaderUniformMap.end();
    }

    size_t Material::GetTextureID(const std::string& textureName)
    {
        return mMaterialBuffer->GetTextureID(textureName);
    }
}
