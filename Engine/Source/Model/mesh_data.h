#ifndef MING3D_MESHDATA_H
#define MING3D_MESHDATA_H

#include "vertex.h"
#include <vector>
#include "texture.h"

namespace Ming3D
{
    class MeshData
    {
    public:
        std::vector<Vertex> mVertices;
        std::vector<unsigned int> mIndices;
        bool mHasNormals = false;
        bool mHasTexCoords = false;
    };
}

#endif
