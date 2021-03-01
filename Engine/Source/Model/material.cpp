#include "material.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "shader_parser.h"
#include "Texture/texture.h"
#include "shader_info.h"
#include "shader_uniform_data.h"
#include "Debug/st_assert.h"
#include "material_factory.h"
#include "SceneRenderer/scene_renderer.h" // TODO

namespace Ming3D
{
    Material::Material(Rendering::ParsedShaderProgram* shaderProgram)
    {
		InitMaterial(shaderProgram);
    }

    Material::Material(Material* otherMat)
    {
        mMaterialParams = otherMat->mMaterialParams;
        Rendering::ParsedShaderProgram* newShaderProg = MaterialFactory::GetParsedShaderProgram(mMaterialParams);
        InitMaterial(newShaderProg);
        mMaterialBuffer->CopyFrom(otherMat->mMaterialBuffer);
    }

    Material::~Material()
    {
    }

	void Material::InitMaterial(Rendering::ParsedShaderProgram* shaderProgram)
	{
		size_t numTextures = shaderProgram->mShaderTextures.size();

		MaterialBuffer* oldMatBuffer = mMaterialBuffer;
		std::vector<Texture*> oldTextures = mTextures;

		// create new material buffer
		mMaterialBuffer = new MaterialBuffer();
		mMaterialBuffer->mTextureBuffers.resize(numTextures);

		mTextures.resize(numTextures);

		// set textures to null
		for (size_t iTexture = 0; iTexture < numTextures; iTexture++)
		{
			mTextures[iTexture] = nullptr;
			mMaterialBuffer->mTextureBuffers[iTexture] = nullptr;
			mMaterialBuffer->mTextureIDs[shaderProgram->mShaderTextures[iTexture].mTextureName] = iTexture;
		}

		// copy old textures (if any)
		if (oldMatBuffer != nullptr)
		{
			for (auto texIter : oldMatBuffer->mTextureIDs)
			{
				const std::string texName = texIter.first;
				size_t texIDOld = texIter.second;
				auto texIDIter = mMaterialBuffer->mTextureIDs.find(texName);
				if (texIDIter != mMaterialBuffer->mTextureIDs.end())
				{
					size_t texIDNew = texIDIter->second;
					mMaterialBuffer->mTextureBuffers[texIDNew] = oldMatBuffer->mTextureBuffers[texIDOld];
					mTextures[texIDNew] = oldTextures[texIDOld];
				}
			}
		}

		int zeroValues[128] = { }; // zero-initialised

		// set constant buffers
		for (size_t iCB = 0; iCB < shaderProgram->mConstantBufferInfos.size(); iCB++)
		{
			const Rendering::ConstantBufferInfo& constantBuffer = shaderProgram->mConstantBufferInfos[iCB];
			mMaterialBuffer->mConstantBuffers.insert(constantBuffer.mName);
		}

		// set uniforms
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

		// Copy cached data from old material buffer, and delete it.
		if (oldMatBuffer != nullptr)
		{
			mMaterialBuffer->CopyFrom(oldMatBuffer);
			delete oldMatBuffer;
		}

		// TODO: Queue render thread command
		mMaterialBuffer->mShaderProgram = GGameEngine->GetRenderDevice()->CreateShaderProgram(shaderProgram);
		GGameEngine->GetSceneRenderer()->RegisterMaterial(mMaterialBuffer); // TODO
	}

	void Material::RecreateMaterial()
	{
		Rendering::ParsedShaderProgram* newShaderProg = MaterialFactory::GetParsedShaderProgram(mMaterialParams);
		InitMaterial(newShaderProg);
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

	void Material::SetColour(glm::vec4 colour)
	{
		SetShaderUniformVec4("_colourDiffuse", colour);
	}

    void Material::SetCastShadows(bool castShadows)
    {
        // TODO: Queue render trhead command
        mMaterialBuffer->mCastShadows = castShadows;

		if (mMaterialBuffer->mRenderType == ERenderType::Transparent)
		{
			SetRenderType(ERenderType::Opaque);
			LOG_WARNING() << "Material::SetCastShadows called on transparent material. Automatically setting render type to Opaque.";
		}
    }

	void Material::SetReceiveShadows(bool receiveShadows)
	{
		// TODO: Queue render trhead command
		mMaterialBuffer->mReceiveShadows = receiveShadows;

		if (receiveShadows)
			EnablePreprocessorDefinition("receive_shadows");
		else
			DisablePreprocessorDefinition("receive_shadows");
	}

	void Material::SetRenderType(ERenderType renderType)
	{
		// TODO: Queue render thread command
		mMaterialBuffer->mRenderType = renderType;
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

	void Material::SetPreprocessorDefinition(const std::string& inName, const std::string& inVal)
	{
		mMaterialParams.mPreprocessorDefinitions.emplace(inName, inVal);
		RecreateMaterial();
	}

	void Material::EnablePreprocessorDefinition(const std::string& inName)
	{
		mMaterialParams.mPreprocessorDefinitions.emplace(inName, "");
		RecreateMaterial();
	}

	void Material::DisablePreprocessorDefinition(const std::string& inName)
	{
		mMaterialParams.mPreprocessorDefinitions.erase(inName);
		RecreateMaterial();
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
