#include "box_collider_component.h"

#include "Actors/actor.h"
#include "GameEngine/game_engine.h"
#include "Physics/physics_manager.h"

#ifdef MING3D_PHYSX
#include "PxRigidActor.h"
#include "PxPhysics.h"
#include "PxShape.h"
#include "PxMaterial.h"
#include "extensions/PxRigidActorExt.h"
#include "Physics/API/PhysX/physics_manager_physx.h"
#include "Physics/API/PhysX/iphysx_actor.h"
#endif

IMPLEMENT_CLASS(Ming3D::BoxColliderComponent)

namespace Ming3D
{
    BoxColliderComponent::~BoxColliderComponent()
    {
#ifdef MING3D_PHYSX
        if (mPxMaterial != nullptr)
            mPxMaterial->release();
        if (mPxShape != nullptr)
            mPxShape->release();
#endif
    }

    void BoxColliderComponent::InitialiseClass()
    {

    }

    void BoxColliderComponent::InitialiseComponent()
    {
        ColliderComponent::InitialiseComponent();

        mParent->RegisterComponentCallback(ComponentCallbackType::PostMove, this);
    }

    void BoxColliderComponent::InitialTick()
    {
        ColliderComponent::InitialTick();
    }

    void BoxColliderComponent::RecreatePhysicsShape()
    {
        ColliderComponent::RecreatePhysicsShape();

        RigidBodyComponent* rigidBodyComp = GetParent()->GetComponent<RigidBodyComponent>();
        assert(rigidBodyComp);

#ifdef MING3D_PHYSX
        if(mPxMaterial == nullptr)
            mPxMaterial = static_cast<PhysicsManagerPhysX*>(GGameEngine->GetPhysicsManager())->GetPxPhysics()->createMaterial(0.5, 0.5, 0.5);

        const glm::vec3 size = mSize * mParent->GetTransform().GetWorldScale();

        physx::PxRigidActor* pxActor = dynamic_cast<IPhysXActor*>(rigidBodyComp->GetPhysicsActor())->GetRigidActor();
        mPxShape = physx::PxRigidActorExt::createExclusiveShape(*pxActor, physx::PxBoxGeometry(size.x, size.y, size.z), *mPxMaterial);
#endif
    }

    void BoxColliderComponent::UpdatePhysicsShape()
    {
#ifdef MING3D_PHYSX
        if (mPxShape == nullptr)
            return;

        RigidBodyComponent* rigidBodyComp = GetParent()->GetComponent<RigidBodyComponent>();
        assert(rigidBodyComp);

        if (mPxMaterial == nullptr)
            mPxMaterial = static_cast<PhysicsManagerPhysX*>(GGameEngine->GetPhysicsManager())->GetPxPhysics()->createMaterial(0.5, 0.5, 0.5);

        const glm::vec3 size = mSize * mParent->GetTransform().GetWorldScale();
        physx::PxBoxGeometry pxGeom = physx::PxBoxGeometry(size.x, size.y, size.z);

        mPxShape->setGeometry(pxGeom);
#endif
    }

    void BoxColliderComponent::PostMove()
    {
        ColliderComponent::PostMove();

        UpdatePhysicsShape();
    }

    void BoxColliderComponent::SetSize(glm::vec3 size)
    {
        mSize = size;
        UpdatePhysicsShape();
    }
}
