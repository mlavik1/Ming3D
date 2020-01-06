#ifndef MING3D_MATERIALBUFFER_H
#define MING3D_MATERIALBUFFER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include <set>

namespace Ming3D
{
    // forward declarations
    class ShaderProgram;
    class TextureBuffer;
    class ShaderUniformData;

    class MaterialBuffer
    {
    private:
        void UpdateUniformData(const std::string& inName, const void* inData);

    public:
        ShaderProgram* mShaderProgram = nullptr;
        std::vector<TextureBuffer*> mTextureBuffers;
        std::unordered_map<std::string, ShaderUniformData*> mShaderUniformMap;
        std::set<std::string> mConstantBuffers;
        std::set<std::string> mModifiedUniforms;

        void SetShaderUniformFloat(const std::string& inName, float inVal);
        void SetShaderUniformInt(const std::string& inName, int inVal);
        void SetShaderUniformVec2(const std::string& inName, const glm::vec2& inVal);
        void SetShaderUniformVec3(const std::string& inName, const glm::vec3& inVal);
        void SetShaderUniformVec4(const std::string& inName, const glm::vec4& inVal);
        void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4& inVal);
    };
}

#endif
