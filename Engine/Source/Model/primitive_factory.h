#ifndef MING3D_PRIMITIVEFACTORY_H
#define MING3D_PRIMITIVEFACTORY_H

#include <memory>
#include "glm/glm.hpp"

namespace Ming3D
{
    class Mesh;

    class PrimitiveFactory
    {
    public:
        static std::unique_ptr<Mesh> CreateBox(const glm::vec3& boxSize);
        static std::unique_ptr<Mesh> CreatePlane(const glm::vec2& planeSize, unsigned int dimX = 1, unsigned int dimY = 1);
    };
}

#endif
