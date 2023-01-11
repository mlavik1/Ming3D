#include "mesh.h"

namespace Ming3D
{
    Mesh::~Mesh()
    {
        delete mVertexData;

        delete mIndexData;
    }
}
