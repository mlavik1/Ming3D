#include "mesh.h"

namespace Ming3D
{
    Mesh::~Mesh()
    {
        if (mVertexData != nullptr)
            delete mVertexData;

        if (mIndexData != nullptr)
            delete mIndexData;
    }
}
