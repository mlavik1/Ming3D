#ifndef MING3D_PRIMITIVEFACTORY_H
#define MING3D_PRIMITIVEFACTORY_H

#include "glm/glm.hpp"

namespace Ming3D
{
    class Mesh;

    class PrimitiveFactory
    {
    public:
        static Mesh* CreateBox(const glm::vec3& boxSize);
    };
}

#endif
