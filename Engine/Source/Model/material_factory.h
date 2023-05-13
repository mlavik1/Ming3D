#ifndef MING3D_MATERIALFACTORY_H
#define MING3D_MATERIALFACTORY_H

#include "material.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace Ming3D
{
	namespace Rendering
	{
		class ParsedShaderProgram;
	}

    class MaterialFactory
    {
    public:
        static std::unique_ptr<Material> CreateMaterial(const std::string& inShaderProgram);
        static std::unique_ptr<Material> CreateMaterial(const MaterialParams& inParams);
		static Rendering::ParsedShaderProgram* GetParsedShaderProgram(const MaterialParams& inParams);
    };
}

#endif
