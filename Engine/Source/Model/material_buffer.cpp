#include "material_buffer.h"
#include "shader_uniform_data.h"

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
}
