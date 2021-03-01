#ifndef MING3D_MATERIALFACTORY_H
#define MING3D_MATERIALFACTORY_H

#include "material.h"
#include <string>
#include <unordered_map>

namespace Ming3D
{
	namespace Rendering
	{
		class ParsedShaderProgram;
	}

    class MaterialFactory
    {
    private:
        static Material* mDefaultGUIMaterial;

    public:
        static Material* CreateMaterial(const std::string& inShaderProgram);
        static Material* CreateMaterial(const MaterialParams& inParams);
		static Rendering::ParsedShaderProgram* GetParsedShaderProgram(const MaterialParams& inParams);
        static Material* GetDefaultGUIMaterial();
    };
}

#endif
