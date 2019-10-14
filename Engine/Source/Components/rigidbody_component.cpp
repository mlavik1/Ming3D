#include "rigidbody_component.h"

#include "Physics/dynamic_physics_actor.h"
#include "Physics/static_physics_actor.h"
#include "GameEngine/game_engine.h"
#include "Physics/physics_manager.h"
#include "Physics/physics_scene.h"
#include "Actors/actor.h"

#ifdef MING3D_PHYSX
#include "Physics/API/PhysX/dynamic_physics_actor_physx.h"
#include "Physics/API/PhysX/static_physics_actor_physx.h"
#include "PxScene.h"
#include "PxRigidActor.h"
#else
#include "Physics/API/Null/dynamic_physics_actor_null.h"
#include "Physics/API/Null/static_physics_actor_null.h"
#endif

IMPLEMENT_CLASS(Ming3D::RigidBodyComponent)

namespace Ming3D
{
    RigidBodyComponent::RigidBodyComponent()
    {

    }

    RigidBodyComponent::~RigidBodyComponent()
    {
        DestroyPhysicsActor();
    }

    void RigidBodyComponent::InitialiseClass()
    {

    }

    void RigidBodyComponent::InitialiseComponent()
    {
        Component::InitialiseComponent();

        mParent->RegisterComponentCallback(ComponentCallbackType::PostMove, this);
    }

    void RigidBodyComponent::InitialTick()
    {
        Component::InitialTick();

        RecreatePhysicsActor();
    }

    void RigidBodyComponent::RecreatePhysicsActor()
    {
        PhysicsScene* physicsScene = GGameEngine->GetPhysicsManager()->GetPhysicsScene(mScene);

        DestroyPhysicsActor();

        if (mIsStatic) {
#ifdef MING3D_PHYSX
            mPhysicsActor = new StaticPhysicsActorPhysX();
#else
            mPhysicsActor = new StaticPhysicsActorNull();
#endif
        }
        else
        {
#ifdef MING3D_PHYSX
            mPhysicsActor = new DynamicPhysicsActorPhysX();
#else
            mPhysicsActor = new DynamicPhysicsActorNull();
#endif
            static_cast<DynamicPhysicsActor*>(mPhysicsActor)->SetKinematic(mIsKinematic);
            static_cast<DynamicPhysicsActor*>(mPhysicsActor)->SetMass(mMass);
        }

#ifdef MING3D_PHYSX
        dynamic_cast<IPhysXActor*>(mPhysicsActor)->GetRigidActor()->userData = this;
#endif
        physicsScene->RegisterRigidBody(this);

        mPhysicsActor->UpdateTransform(mParent->GetTransform());
    }

    void RigidBodyComponent::DestroyPhysicsActor()
    {
        if (mPhysicsActor)
        {
            PhysicsScene* physicsScene = GGameEngine->GetPhysicsManager()->GetPhysicsScene(mScene);
            physicsScene->UnregisterRigidBody(this);
            delete mPhysicsActor;
        }
    }

    void RigidBodyComponent::SetStatic(bool setStatic)
    {
        if (mIsStatic == setStatic)
            return;

        mIsStatic = setStatic;
        RecreatePhysicsActor();
    }

    void RigidBodyComponent::SetScene(int scene)
    {
        if (mScene == scene)
            return;

        mScene = scene;
        RecreatePhysicsActor();
    }

    void RigidBodyComponent::SetKinematic(bool setKinematic)
    {
        mIsKinematic = setKinematic;
        if (!mIsStatic && mPhysicsActor != nullptr)
        {
            static_cast<DynamicPhysicsActor*>(mPhysicsActor)->SetKinematic(setKinematic);
        }
    }

    void RigidBodyComponent::SetMass(float mass)
    {
        mMass = mass;

        if (!mIsStatic && mPhysicsActor != nullptr)
            static_cast<DynamicPhysicsActor*>(mPhysicsActor)->SetMass(mass);
    }

    void RigidBodyComponent::AddForce(const glm::vec3& force, const ForceMode& forceMode)
    {
        if (!mIsStatic && mPhysicsActor != nullptr)
            static_cast<DynamicPhysicsActor*>(mPhysicsActor)->AddForce(force, forceMode);
    }

    void RigidBodyComponent::AddForceAtPos(const glm::vec3& force, const glm::vec3& pos, const ForceMode& forceMode)
    {
        if (!mIsStatic && mPhysicsActor != nullptr)
            static_cast<DynamicPhysicsActor*>(mPhysicsActor)->AddForceAtPos(force, pos, forceMode);
    }

    void RigidBodyComponent::PostMove()
    {
        Component::PostMove();

        UpdateTransform();
    }

    void RigidBodyComponent::UpdateTransform()
    {
        mPhysicsActor->UpdateTransform(mParent->GetTransform());
    }
}
