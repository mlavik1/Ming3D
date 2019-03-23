#include "material_factory.h"

#include "shader_parser.h"

namespace Ming3D
{
    Material* MaterialFactory::CreateMaterial(const std::string& inShaderProgram)
    {
        ShaderParser parser;
        ParsedShaderProgram* parsedProgram = parser.ParseShaderProgram(inShaderProgram);
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
