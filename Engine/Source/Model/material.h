#ifndef MING3D_MATERIAL_H
#define MING3D_MATERIAL_H

#include "material_buffer.h"
#include <vector>
#include "glm/glm.hpp"

namespace Ming3D
{
    class ParsedShaderProgram;
    class Texture;

    class Material
    {
    public:
        MaterialBuffer* mMaterialBuffer;
        ParsedShaderProgram* mShaderProgramData;
        std::vector<Texture*> mTextures;

        Material(ParsedShaderProgram* shaderProgram);
        ~Material();

        void SetTexture(size_t textureIndex, Texture* texture);

        void SetShaderUniformVec4(const std::string& inName, const glm::vec4& inVal);
        void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4& inVal);
    };
}

#endif
