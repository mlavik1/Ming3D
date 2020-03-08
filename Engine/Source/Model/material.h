#ifndef MING3D_MATERIAL_H
#define MING3D_MATERIAL_H

#include "material_buffer.h"
#include <vector>
#include "glm/glm.hpp"

namespace Ming3D
{
    class Texture;

    namespace Rendering
    {
        class ParsedShaderProgram;
    }

    class Material
    {
    public:
        MaterialBuffer* mMaterialBuffer;
        std::vector<Texture*> mTextures;

        Material(Rendering::ParsedShaderProgram* shaderProgram);
        ~Material();

        void SetTexture(size_t textureIndex, Texture* texture);

        void SetShaderUniformFloat(const std::string& inName, float inVal);
        void SetShaderUniformInt(const std::string& inName, int inVal);
        void SetShaderUniformVec2(const std::string& inName, const glm::vec2& inVal);
        void SetShaderUniformVec3(const std::string& inName, const glm::vec3& inVal);
        void SetShaderUniformVec4(const std::string& inName, const glm::vec4& inVal);
        void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4& inVal);
    };
}

#endif
