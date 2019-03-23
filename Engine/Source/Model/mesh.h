#ifndef MING3D_MESH_H
#define MING3D_MESH_H

#include "vertex.h"
#include <vector>
#include "graphics_data.h"

namespace Ming3D
{
    class Mesh
    {
    public:
        VertexData* mVertexData = nullptr;
        IndexData* mIndexData = nullptr;
        bool mHasNormals = false;
        bool mHasTexCoords = false;

        ~Mesh();
    };
}

#endif
