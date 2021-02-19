#pragma once

#include "Model/mesh_buffer.h"
#include "Model/material_buffer.h"
#include "glm/glm.hpp"

namespace Ming3D
{
    class RenderBatch
    {
    public:
        MeshBuffer* mMeshBuffer;
        glm::mat4 mModelMatrix;
        MaterialBuffer* mMaterial = nullptr;
    };
}
