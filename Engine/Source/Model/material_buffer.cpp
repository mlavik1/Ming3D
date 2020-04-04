#include "material_buffer.h"
#include "shader_uniform_data.h"
#include "Debug/debug.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
    void MaterialBuffer::UpdateUniformData(const std::string& inName, const void* inData)
    {
        auto it = mShaderUniformMap.find(inName);
        if (it != mShaderUniformMap.end())
        {
            it->second->SetData(inData);
            mModifiedUniforms.insert(inName);
        }
        else
            LOG_ERROR() << "Failed to find uniform with name: " << inName;
    }

	void MaterialBuffer::CopyFrom(MaterialBuffer* otherMat)
	{
		this->mCastShadows = otherMat->mCastShadows;
		this->mReceiveShadows = otherMat->mReceiveShadows;

		// copy uniform data
		for (auto otherUniformIt : otherMat->mShaderUniformMap)
		{
			auto thisUniformIt = mShaderUniformMap.find(otherUniformIt.first);
			if (thisUniformIt != mShaderUniformMap.end())
			{
				thisUniformIt->second->CopyData(otherUniformIt.second);
				mModifiedUniforms.emplace(thisUniformIt->first);
			}
		}
	}

    void MaterialBuffer::SetShaderUniformFloat(const std::string& inName, float inVal)
    {
        UpdateUniformData(inName, &inVal);
    }

    void MaterialBuffer::SetShaderUniformInt(const std::string& inName, int inVal)
    {
        UpdateUniformData(inName, &inVal);
    }

    void MaterialBuffer::SetShaderUniformVec2(const std::string& inName, const glm::vec2& inVal)
    {
        UpdateUniformData(inName, &inVal);
    }

    void MaterialBuffer::SetShaderUniformVec3(const std::string& inName, const glm::vec3& inVal)
    {
        UpdateUniformData(inName, &inVal);
    }

    void MaterialBuffer::SetShaderUniformVec4(const std::string& inName, const glm::vec4& inVal)
    {
        UpdateUniformData(inName, &inVal);
    }

    void MaterialBuffer::SetShaderUniformMat4x4(const std::string& inName, const glm::mat4& inVal)
    {
        UpdateUniformData(inName, &inVal);
    }

    size_t MaterialBuffer::GetTextureID(const std::string& textureName)
    {
        auto it = mTextureIDs.find(textureName);
        __Assert(it != mTextureIDs.end());
        return it->second;
    }
}
