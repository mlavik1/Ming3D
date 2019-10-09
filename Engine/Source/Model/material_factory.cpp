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
        ShaderParserParams params;
        params.mShaderProgramPath = inParams.mShaderProgramPath;
        params.mPreprocessorDefinitions = inParams.mPreprocessorDefinitions;
        ParsedShaderProgram* parsedProgram = nullptr;
        if (!ShaderCache::GetCachedProgramInfo(params, parsedProgram))
        {
            ShaderParser parser;
            parsedProgram = parser.ParseShaderProgram(params);
            ShaderCache::CacheProgramInfo(params, parsedProgram);
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
