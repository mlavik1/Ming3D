#ifndef MING3D_RENDERSCENEOBJECT_H
#define MING3D_RENDERSCENEOBJECT_H

#include "Model/mesh_buffer.h"
#include "Components/component.h"
#include "Model/material_buffer.h"
#include "glm/glm.hpp"

namespace Ming3D
{
    class MeshBuffer;

    class RenderSceneObject
    {
    public:
        MeshBuffer* mMesh;
        glm::mat4 mModelMatrix;
        MaterialBuffer* mMaterial = nullptr;
    };
}

#endif
