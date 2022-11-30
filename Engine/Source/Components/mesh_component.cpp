#include "mesh_component.h"

#include "Model/model_helper.h"
#include "render_device.h"
#include "Actors/actor.h"
#include "Model/material_factory.h"
#include "render_device.h"
#include "World/world.h"
#include "SceneRenderer/render_scene.h"

IMPLEMENT_CLASS(Ming3D::MeshComponent)

namespace Ming3D
{
    MeshComponent::MeshComponent()
    {
        mRenderObject = new MeshRenderObject();
    }

    MeshComponent::~MeshComponent()
    {
        GetWorld()->GetRenderScene()->RemoveSceneObject(mRenderObject);
        delete mRenderObject;
    }

    void MeshComponent::InitialiseClass()
    {

    }

    void MeshComponent::InitialiseComponent()
    {
        Super::InitialiseComponent();
        GetWorld()->GetRenderScene()->AddSceneObject(mRenderObject);
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
