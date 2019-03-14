#include "box_collider_component.h"

#include "Actors/actor.h"
#include "PxRigidActor.h"
#include "GameEngine/game_engine.h"
#include "Physics/physics_manager.h"
#include "PxPhysics.h"
#include "PxShape.h"

IMPLEMENT_CLASS(Ming3D::BoxColliderComponent)

namespace Ming3D
{
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

        if(mPxMaterial == nullptr)
            mPxMaterial = GGameEngine->GetPhysicsManager()->GetPxPhysics()->createMaterial(0.5, 0.5, 0.5);

        const glm::vec3 size = mSize * mParent->GetTransform().GetWorldScale();

        physx::PxRigidActor* pxActor = rigidBodyComp->GetPhysicsActor()->GetRigidActor();
        mPxShape = pxActor->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *mPxMaterial);
    }

    void BoxColliderComponent::UpdatePhysicsShape()
    {
        if (mPxShape == nullptr)
            return;

        RigidBodyComponent* rigidBodyComp = GetParent()->GetComponent<RigidBodyComponent>();
        assert(rigidBodyComp);

        if (mPxMaterial == nullptr)
            mPxMaterial = GGameEngine->GetPhysicsManager()->GetPxPhysics()->createMaterial(0.5, 0.5, 0.5);

        const glm::vec3 size = mSize * mParent->GetTransform().GetWorldScale();
        physx::PxBoxGeometry pxGeom = physx::PxBoxGeometry(size.x, size.y, size.z);

        physx::PxRigidActor* pxActor = rigidBodyComp->GetPhysicsActor()->GetRigidActor();
        mPxShape->setGeometry(pxGeom);
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
