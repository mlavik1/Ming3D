#ifndef MING3D_MESH_H
#define MING3D_MESH_H

#include <vector>
#include "graphics_data.h"

namespace Ming3D
{
    class Mesh
    {
    public:
        VertexData* mVertexData = nullptr;
        IndexData* mIndexData = nullptr;

        ~Mesh();
    };
}

#endif
