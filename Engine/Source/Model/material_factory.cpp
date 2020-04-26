#include "material_factory.h"

#include "shader_parser.h"
#include "shader_cache.h"

namespace Ming3D
{
    Material* MaterialFactory::CreateMaterial(const std::string& inShaderProgram)
    {
        MaterialParams params;
        params.mShaderProgramPath = inShaderProgram;
        return CreateMaterial(params);
    }

    Material* MaterialFactory::CreateMaterial(const MaterialParams& inParams)
    {
		Rendering::ParsedShaderProgram* parsedProgram = GetParsedShaderProgram(inParams);
        if (parsedProgram != nullptr)
        {
            Material* mat = new Material(parsedProgram);
			mat->mMaterialParams = inParams;
            // Set default uniform values
            if(mat->HasShaderUniform("_textureTiling"))
                mat->SetShaderUniformVec2("_textureTiling", glm::vec2(1.0f, 1.0f));
			if (mat->HasShaderUniform("_colourDiffuse"))
				mat->SetShaderUniformVec4("_colourDiffuse", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			if (mat->HasShaderUniform("_colourSpecular"))
				mat->SetShaderUniformVec4("_colourSpecular", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			if (mat->HasShaderUniform("_shininess"))
				mat->SetShaderUniformFloat("_shininess", 1.0f);
            return mat;
        }
        else
        {
            return nullptr;
        }
    }

	Rendering::ParsedShaderProgram* MaterialFactory::GetParsedShaderProgram(const MaterialParams& inParams)
	{
		Rendering::ShaderParserParams params;
		params.mShaderProgramPath = inParams.mShaderProgramPath;
		params.mPreprocessorDefinitions = inParams.mPreprocessorDefinitions;

		Rendering::ParsedShaderProgram* parsedProgram = nullptr;
		if (!Rendering::ShaderCache::GetCachedProgramInfo(params, parsedProgram))
		{
			Rendering::ShaderParser parser;
			parsedProgram = parser.ParseShaderProgram(params);
			Rendering::ShaderCache::CacheProgramInfo(params, parsedProgram);
		}
		return parsedProgram;
	}
}
