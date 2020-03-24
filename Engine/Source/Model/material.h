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

    /**
     * @brief Material class.
     * Engine-side wrapper for a shader program and all textures and uniforms (and their data).
     */
    class Material
    {
    private:
        std::vector<Texture*> mTextures;

    public:
        MaterialBuffer* mMaterialBuffer;

        Material(Rendering::ParsedShaderProgram* shaderProgram);
        ~Material();

        void SetTexture(size_t textureIndex, Texture* texture);
        void SetTexture(const std::string& textureName, Texture* texture);

        void SetCastShadows(bool castShadows);

        void SetShaderUniformFloat(const std::string& inName, float inVal);
        void SetShaderUniformInt(const std::string& inName, int inVal);
        void SetShaderUniformVec2(const std::string& inName, const glm::vec2& inVal);
        void SetShaderUniformVec3(const std::string& inName, const glm::vec3& inVal);
        void SetShaderUniformVec4(const std::string& inName, const glm::vec4& inVal);
        void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4& inVal);

        bool HasShaderUniform(const std::string& inName);

        /**
         * @brief Gets the ID (binding) of a texture
         * @param textureName name of the texture (in shader)
         * @return Texture ID (OpenGL: binding). Use this when setting the texture through the RenderDevice.
         */
        size_t GetTextureID(const std::string& textureName);
    };
}

#endif
