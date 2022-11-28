#include "mesh_render_object.h"
#include "Model/mesh.h"
#include "Model/material.h"
#include "Model/mesh_buffer.h"
#include "Model/material_buffer.h"
#include "render_batch.h"
#include "render_device.h"
#include "GameEngine/game_engine.h"
#include "Actors/transform.h"

namespace Ming3D
{
    void MeshRenderObject::SetMesh(Mesh* mesh, bool dynamic)
    {
        Rendering::RenderDevice* renderDevice = GGameEngine->GetRenderDevice();

        if (mMeshBuffer == nullptr)
            mMeshBuffer = new MeshBuffer();

        if (mMeshBuffer->mVertexBuffer != nullptr)
            delete mMeshBuffer->mVertexBuffer;
        if (mMeshBuffer->mIndexBuffer != nullptr)
            delete mMeshBuffer->mIndexBuffer;

        Rendering::EBufferUsage bufferUsage = dynamic ? Rendering::EBufferUsage::DynamicDraw : Rendering::EBufferUsage::StaticDraw;

        mMeshBuffer->mVertexBuffer = renderDevice->CreateVertexBuffer(mesh->mVertexData, bufferUsage);
        mMeshBuffer->mIndexBuffer = renderDevice->CreateIndexBuffer(mesh->mIndexData, bufferUsage);
    }

    void MeshRenderObject::SetMaterial(Material* material)
    {
        mMaterialBuffer = material->mMaterialBuffer;
    }

    void MeshRenderObject::SetTransform(glm::mat4 trans)
    {
        mModelMatrix = trans;
    }

    void MeshRenderObject::ReuploadVertexData(Mesh* mesh)
    {
        Rendering::RenderDevice* renderDevice = GGameEngine->GetRenderDevice();
        renderDevice->UpdateVertexBuffer(mMeshBuffer->mVertexBuffer, mesh->mVertexData);
    }

    int MeshRenderObject::GetNumBatches()
    {
        return mMeshBuffer != nullptr ? 1 : 0;
    }

    void MeshRenderObject::GetRenderBatch(int batchIndex, RenderBatch* outBatch)
    {
        outBatch->mMeshBuffer = mMeshBuffer;
        outBatch->mMaterial = mMaterialBuffer;
        outBatch->mModelMatrix = mModelMatrix;
        outBatch->mStartIndex = 0;
        outBatch->mNumIndices = mMeshBuffer->mIndexBuffer->GetNumIndices();
    }

    ERenderType MeshRenderObject::GetRenderType()
    {
        return mMaterialBuffer->mRenderType;
    }
    bool MeshRenderObject::GetCastShadows()
    {
        return mMaterialBuffer->mCastShadows;
    }
    glm::vec3 MeshRenderObject::GetWorldPosition()
    {
        return mModelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
