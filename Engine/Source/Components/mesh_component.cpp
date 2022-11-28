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
        mRenderObject = new MeshRenderObject();
        GGameEngine->GetSceneRenderer()->AddSceneObject(mRenderObject);
    }

    MeshComponent::~MeshComponent()
    {
        delete mRenderObject;
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
        mRenderObject->SetMesh(inMesh, dynamic);
    }

    void MeshComponent::SetMaterial(Material* inMat)
    {
        mMaterial = inMat;
        mRenderObject->SetMaterial(inMat);
    }

    void MeshComponent::ReuploadVertexData()
    {
        mRenderObject->ReuploadVertexData(mMesh);
    }

    void MeshComponent::Tick(float inDeltaTime)
    {
        Component::Tick(inDeltaTime);
        mRenderObject->SetTransform(mParent->GetTransform().GetWorldTransformMatrix());
    }
}
