#ifndef MING3D_MATERIALFACTORY_H
#define MING3D_MATERIALFACTORY_H

#include "material.h"
#include <string>

namespace Ming3D
{
    class MaterialFactory
    {
    public:
        static Material* CreateMaterial(const std::string& inShaderProgram);
    };
}

#endif
