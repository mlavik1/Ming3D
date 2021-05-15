#include "mesh_component.h"

#include "Model/model_helper.h"
#include "SceneRenderer/render_scene.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "SceneRenderer/scene_renderer.h"
#include "Actors/actor.h"
#include "Model/material_factory.h"
#include "render_device.h"

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

    void MeshComponent::SetMesh(Mesh* inMesh, bool dynamic)
    {
        mMesh = inMesh;

        Rendering::RenderDevice* renderDevice = GGameEngine->GetRenderDevice();
        MeshBuffer* meshBuffer = new MeshBuffer();

        // TODO: Store vertex layout in mesh
        Rendering::VertexData* vertexData = inMesh->mVertexData;
        Rendering::IndexData* indexData(inMesh->mIndexData);

        meshBuffer->mVertexBuffer = renderDevice->CreateVertexBuffer(vertexData, dynamic ? Rendering::EVertexBufferUsage::DynamicDraw : Rendering::EVertexBufferUsage::StaticDraw);
        meshBuffer->mIndexBuffer = renderDevice->CreateIndexBuffer(indexData);

        mRenderSceneObject->mModelMatrix = mParent->GetTransform().GetWorldTransformMatrix();
        mRenderSceneObject->mMesh = meshBuffer;

        GGameEngine->GetSceneRenderer()->AddSceneObject(mRenderSceneObject);
    }

    void MeshComponent::SetMaterial(Material* inMat)
    {
        mMaterial = inMat;

        mRenderSceneObject->mMaterial = mMaterial->mMaterialBuffer;
    }

    void MeshComponent::ReuploadVertexData()
    {
        Rendering::RenderDevice* renderDevice = GGameEngine->GetRenderDevice();
        renderDevice->UpdateVertexBuffer(mRenderSceneObject->mMesh->mVertexBuffer, mMesh->mVertexData);
    }

    void MeshComponent::Tick(float inDeltaTime)
    {
        Component::Tick(inDeltaTime);

        mRenderSceneObject->mModelMatrix = mParent->GetTransform().GetWorldTransformMatrix();
    }
}
