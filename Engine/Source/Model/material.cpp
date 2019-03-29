#include "material.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "shader_parser.h"
#include "texture.h"
#include "shader_info.h"
#include "shader_uniform_data.h"

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

        int zeroValues[128] = { }; // zero-initialised

        for (size_t iBlock = 0; iBlock < shaderProgram->mShaderUniformBlocks.size(); iBlock++)
        {
            const ShaderUniformBlock& uniformBlock = shaderProgram->mShaderUniformBlocks[iBlock];
            for (size_t iUniform = 0; iUniform < uniformBlock.mShaderUniforms.size(); iUniform++)
            {
                const ShaderVariableInfo& uniform = uniformBlock.mShaderUniforms[iUniform];
                ShaderUniformData* uniformData = new ShaderUniformData(uniform.mDatatypeInfo, uniform.mDatatypeInfo.GetDataSize());
                switch (uniform.mDatatypeInfo.mDatatype)
                {
                case EShaderDatatype::Mat4x4:
                    uniformData->SetData(&glm::mat4(1.0f));
                    break;
                default:
                    uniformData->SetData(&zeroValues);
                }
                mMaterialBuffer->mShaderUniformMap.emplace(uniform.mName, uniformData);
            }
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
}
