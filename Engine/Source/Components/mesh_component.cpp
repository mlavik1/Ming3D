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
        VertexData* vertexData = inMesh->mVertexData;
        IndexData* indexData(inMesh->mIndexData);

        meshBuffer->mVertexBuffer = renderDevice->CreateVertexBuffer(vertexData);
        meshBuffer->mIndexBuffer = renderDevice->CreateIndexBuffer(indexData);

        mRenderSceneObject->mModelMatrix = mParent->GetTransform().GetWorldTransformMatrix();
        mRenderSceneObject->mMeshes.push_back(meshBuffer);

        GGameEngine->GetSceneRenderer()->AddSceneObject(mRenderSceneObject);
    }

    void MeshComponent::SetMaterial(Material* inMat)
    {
        mMaterial = inMat;

        mRenderSceneObject->mMaterial = mMaterial->mMaterialBuffer;
    }

    void MeshComponent::Tick(float inDeltaTime)
    {
        Component::Tick(inDeltaTime);

        mRenderSceneObject->mModelMatrix = mParent->GetTransform().GetWorldTransformMatrix();
    }
}
