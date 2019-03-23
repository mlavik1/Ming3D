#ifndef MING3D_MESH_H
#define MING3D_MESH_H

#include "vertex.h"
#include <vector>

namespace Ming3D
{
    class Mesh
    {
    public:
        std::vector<Vertex> mVertices;
        std::vector<unsigned int> mIndices;
        bool mHasNormals = false;
        bool mHasTexCoords = false;
    };
}

#endif
