#ifndef MING3D_RENDERSCENEOBJECT_H
#define MING3D_RENDERSCENEOBJECT_H

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture_buffer.h"
#include "shader_program.h"
#include "Model/mesh_buffer.h"
#include "Components/component.h"
#include "Model/material_buffer.h"

namespace Ming3D
{
    class RenderSceneObject
    {
    public:
        Component* mOwnerComponent = nullptr;
        std::vector<MeshBuffer*> mMeshes;
        glm::mat4 mModelMatrix;
        MaterialBuffer* mMaterial = nullptr;
    };
}

#endif
