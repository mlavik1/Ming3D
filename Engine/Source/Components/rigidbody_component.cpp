#include "rigidbody_component.h"

#include "Physics/dynamic_physics_actor.h"
#include "Physics/static_physics_actor.h"
#include "GameEngine/game_engine.h"
#include "Physics/physics_manager.h"
#include "Physics/physics_scene.h"
#include "PxScene.h"
#include "PxRigidActor.h"
#include "Actors/actor.h"
#include "Physics/physx_conversions.h"

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

        if (mIsStatic)
            mPhysicsActor = new StaticPhysicsActor();
        else
        {
            mPhysicsActor = new DynamicPhysicsActor();
            ((DynamicPhysicsActor*)mPhysicsActor)->SetKinematic(mIsKinematic);
        }

        mPhysicsActor->GetRigidActor()->userData = this;
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

    void RigidBodyComponent::SetKinematic(bool setKinematic)
    {
        mIsKinematic = setKinematic;
        if (!mIsStatic && mPhysicsActor != nullptr)
        {
            ((DynamicPhysicsActor*)mPhysicsActor)->SetKinematic(setKinematic);
        }
    }

    void RigidBodyComponent::SetScene(int scene)
    {
        if (mScene == scene)
            return;

        mScene = scene;
        RecreatePhysicsActor();
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
