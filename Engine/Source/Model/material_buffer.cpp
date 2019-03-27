#include "material_buffer.h"

namespace Ming3D
{
    void MaterialBuffer::UpdateUniformData(const std::string& inName, void* inData)
    {
        auto it = mShaderUniformMap.find(inName);
        if (it != mShaderUniformMap.end())
        {
            it->second->SetData(inData);
            mModifiedUniforms.insert(inName);
        }
    }

    void MaterialBuffer::SetShaderUniformVec4(const std::string& inName, glm::vec4 inVal)
    {
        UpdateUniformData(inName, &inVal);
    }

    void MaterialBuffer::SetShaderUniformMat4x4(const std::string& inName, glm::mat4 inVal)
    {
        UpdateUniformData(inName, &inVal);
    }
}
