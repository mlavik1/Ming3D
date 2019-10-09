#ifndef MING3D_MATERIALFACTORY_H
#define MING3D_MATERIALFACTORY_H

#include "material.h"
#include <string>
#include <unordered_map>

namespace Ming3D
{
    struct MaterialParams
    {
        std::string mShaderProgramPath;
        std::unordered_map<std::string, std::string> mPreprocessorDefinitions;
    };

    class MaterialFactory
    {
    public:
        static Material* CreateMaterial(const std::string& inShaderProgram);
        static Material* CreateMaterial(const MaterialParams& inParams);
    };
}

#endif
