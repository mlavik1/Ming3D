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
        if (parsedProgram != nullptr)
        {
            Material* mat = new Material(parsedProgram);
            return mat;
        }
        else
        {
            return nullptr;
        }
    }
}
