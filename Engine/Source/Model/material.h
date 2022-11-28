#ifndef MING3D_MATERIAL_H
#define MING3D_MATERIAL_H

#include "material_buffer.h"
#include <vector>
#include "glm/glm.hpp"
#include "SceneRenderer/render_type.h"
#include <memory>

namespace Ming3D
{
    class Texture;

    namespace Rendering
    {
        class ParsedShaderProgram;
    }

	struct MaterialParams
	{
		std::string mShaderProgramPath;
		std::unordered_map<std::string, std::string> mPreprocessorDefinitions;
	};

    /**
     * @brief Material class.
     * Engine-side wrapper for a shader program and all textures and uniforms (and their data).
     */
    class Material
    {
    private:
        std::vector<std::shared_ptr<Texture>> mTextures;

		/**
		 * @brief Initialises the material. This can be done on a new material or an old material.
		   All texture, uniforms and constant buffers will be copied.
		 * @param shaderProgram parsed shader program. You can get one from the MaterialFactory.
		 */
		void InitMaterial(Rendering::ParsedShaderProgram* shaderProgram);

		/**
		 * @brief Recreates the material based on the current MaterialParams of the material.
		 * This will change the currently used shader program.
		 */
		void RecreateMaterial();

    public:
        MaterialBuffer* mMaterialBuffer = nullptr;
		MaterialParams mMaterialParams;

        Material(Rendering::ParsedShaderProgram* shaderProgram);
        Material(Material* otherMat);
        ~Material();

        void SetTexture(size_t textureIndex, std::shared_ptr<Texture> texture);
        void SetTexture(const std::string& textureName, std::shared_ptr<Texture> texture);
		void SetColour(glm::vec4 colour);

        void SetCastShadows(bool castShadows);
		void SetReceiveShadows(bool receiveShadows);
		void SetRenderType(ERenderType renderType);

        void SetShaderUniformFloat(const std::string& inName, float inVal);
        void SetShaderUniformInt(const std::string& inName, int inVal);
        void SetShaderUniformVec2(const std::string& inName, const glm::vec2& inVal);
        void SetShaderUniformVec3(const std::string& inName, const glm::vec3& inVal);
        void SetShaderUniformVec4(const std::string& inName, const glm::vec4& inVal);
        void SetShaderUniformMat4x4(const std::string& inName, const glm::mat4& inVal);

		void SetPreprocessorDefinition(const std::string& inName, const std::string& inVal);
		void EnablePreprocessorDefinition(const std::string& inName);
		void DisablePreprocessorDefinition(const std::string& inName);

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
