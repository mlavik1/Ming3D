#ifndef MING3D_MATERIALBUFFER_H
#define MING3D_MATERIALBUFFER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "shader_uniform_data.h"
#include "glm/glm.hpp"
#include <set>

namespace Ming3D
{
    // forward declarations
    class ShaderProgram;
    class TextureBuffer;

    class MaterialBuffer
    {
    private:
        void UpdateUniformData(const std::string& inName, void* inData);

    public:
        ShaderProgram* mShaderProgram = nullptr;
        std::vector<TextureBuffer*> mTextureBuffers;
        std::unordered_map<std::string, ShaderUniformData*> mShaderUniformMap;
        std::set<std::string> mModifiedUniforms;

        void SetShaderUniformVec4(const std::string& inName, glm::vec4 inVal);
        void SetShaderUniformMat4x4(const std::string& inName, glm::mat4 inVal);
    };
}

#endif
