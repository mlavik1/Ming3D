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
    class ShaderUniformData;
    namespace Rendering
    {
        class ShaderProgram;
        class TextureBuffer;
    }

    /**
     * @brief Container for (compiled) ShaderProgram, TextureBuffers, etc.
     */
    class MaterialBuffer
    {
    private:
        void UpdateUniformData(const std::string& inName, const void* inData);

    public:
        Rendering::ShaderProgram* mShaderProgram = nullptr;
        /* List of all texture buffers (render API side texture). */
        std::vector<Rendering::TextureBuffer*> mTextureBuffers;
        /* Maps uniform name to uniform buffered data. */
        std::unordered_map<std::string, ShaderUniformData*> mShaderUniformMap;
        /* List of all constant buffer names. */
        std::set<std::string> mConstantBuffers;
        /* List of uniforms that have been modified the last frame. */
        std::set<std::string> mModifiedUniforms;
        /* Maps texture name to ID (OpenGL: binding). */
        std::unordered_map<std::string, size_t> mTextureIDs;
        /* Defines whether the object will cast shadows. */
        bool mCastShadows = true;

        void SetShaderUniformFloat(const std::string& inName, float inVal);
        void SetShaderUniformInt(const std::string& inName, int inVal);
        void SetShaderUniformVec2(const std::string& inName, const glm::vec2& inVal);
        void SetShaderUniformVec3(const std::string& inName, const glm::vec3& inVal);
        void SetShaderUniformVec4(const std::string& inName, const glm::vec4& inVal);
        void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4& inVal);

        /**
         * @brief Gets the ID (binding) of a texture
         * @param textureName name of the texture (in shader)
         * @return Texture ID (OpenGL: binding). Use this when setting the texture through the RenderDevice.
         */
        size_t GetTextureID(const std::string& textureName);
    };
}

#endif
