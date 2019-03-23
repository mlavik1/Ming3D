#include "mesh_component.h"

#include "Model/model_helper.h"
#include "SceneRenderer/render_scene.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "SceneRenderer/scene_renderer.h"
#include "Actors/actor.h"
#include "Model/material_factory.h"

IMPLEMENT_CLASS(Ming3D::MeshComponent)

namespace Ming3D
{
    MeshComponent::MeshComponent()
    {
        mRenderSceneObject = new RenderSceneObject();
    }

    void MeshComponent::InitialiseClass()
    {

    }

    void MeshComponent::InitialiseComponent()
    {
        Super::InitialiseComponent();
    }

    void MeshComponent::SetMesh(Mesh* inMesh)
    {
        mMesh = inMesh;

        RenderDevice* renderDevice = GGameEngine->GetRenderDevice();
        MeshBuffer* meshBuffer = new MeshBuffer();

        // TODO: Store vertex layout in mesh
        VertexData vertexData({ EVertexComponent::Position, EVertexComponent::Normal, EVertexComponent::TexCoord }, inMesh->mVertices.size());
        IndexData indexData(inMesh->mIndices.size());

        memcpy(vertexData.GetDataPtr(), inMesh->mVertices.data(), inMesh->mVertices.size() * sizeof(Vertex));
        memcpy(indexData.GetData(), inMesh->mIndices.data(), inMesh->mIndices.size() * sizeof(unsigned int));

        meshBuffer->mVertexBuffer = renderDevice->CreateVertexBuffer(&vertexData);
        meshBuffer->mIndexBuffer = renderDevice->CreateIndexBuffer(&indexData);

        mRenderSceneObject->mOwnerComponent = this;
        mRenderSceneObject->mModelMatrix = mParent->GetTransform().GetWorldTransformMatrix();
        mRenderSceneObject->mMeshes.push_back(meshBuffer);

        GGameEngine->GetSceneRenderer()->AddSceneObject(mRenderSceneObject);
    }

    void MeshComponent::SetMaterial(Material* inMat)
    {
        mMaterial = inMat;

        mRenderSceneObject->mMaterial = mMaterial->mMaterialBuffer;
    }
}
