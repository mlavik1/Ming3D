#ifndef MING3D_VERTEX_H
#define MING3D_VERTEX_H

#include "glm/glm.hpp"

namespace Ming3D
{
    class Vertex
    {
    public:
        glm::vec3 mVertex;
        glm::vec3 mNormal;
        glm::vec2 mTexCoord;
    };
}

#endif
