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

    void MeshComponent::SetMesh(std::shared_ptr<Mesh> inMesh, bool dynamic)
    {
        mMesh = inMesh;
        mRenderObject->SetMesh(inMesh.get(), dynamic);
    }

    void MeshComponent::SetMaterial(std::shared_ptr<Material> inMat)
    {
        mMaterial = inMat;
        mRenderObject->SetMaterial(inMat.get());
    }

    void MeshComponent::ReuploadVertexData()
    {
        mRenderObject->ReuploadVertexData(mMesh.get());
    }

    void MeshComponent::Tick(float inDeltaTime)
    {
        Component::Tick(inDeltaTime);
        mRenderObject->SetTransform(mParent->GetTransform().GetWorldTransformMatrix());
    }
}
