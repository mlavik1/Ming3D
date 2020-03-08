#ifndef MING3D_MESH_H
#define MING3D_MESH_H

#include <vector>
#include "graphics_data.h"

namespace Ming3D
{
    class Mesh
    {
    public:
        Rendering::VertexData* mVertexData = nullptr;
        Rendering::IndexData* mIndexData = nullptr;

        ~Mesh();
    };
}

#endif
