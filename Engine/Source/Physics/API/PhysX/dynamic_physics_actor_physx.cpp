#ifdef MING3D_PHYSX
#include "dynamic_physics_actor_physx.h"

#include "PxRigidDynamic.h"
#include "GameEngine/game_engine.h"
#include "physics_manager_physx.h"
#include "PxPhysics.h"
#include "physx_conversions.h"
#include "extensions/PxRigidBodyExt.h"

namespace Ming3D
{
    DynamicPhysicsActorPhysX::DynamicPhysicsActorPhysX()
    {
        physx::PxTransform trans(physx::PxVec3(0.0f, 0.0f, 0.0f));
        mPxRigidDynamic = static_cast<PhysicsManagerPhysX*>(GGameEngine->GetPhysicsManager())->GetPxPhysics()->createRigidDynamic(trans);
    }

    physx::PxRigidActor* DynamicPhysicsActorPhysX::GetRigidActor()
    {
        return mPxRigidDynamic;
    }

    void DynamicPhysicsActorPhysX::UpdateTransform(const Transform& inTrans)
    {
        physx::PxTransform trans(PhysXConversions::glmVec3ToPxVec3(inTrans.GetWorldPosition()), PhysXConversions::glmQuatToPxQuat(inTrans.GetWorldRotation()));

        if (!mIsKinematic)
        {
            mPxRigidDynamic->setGlobalPose(trans);
        }
        else
        {
            mPxRigidDynamic->setKinematicTarget(trans);
        }
    }

    void DynamicPhysicsActorPhysX::SetKinematic(bool inKinematic)
    {
        mIsKinematic = inKinematic;
        mPxRigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, inKinematic);
    }

    void DynamicPhysicsActorPhysX::SetMass(float mass)
    {
        mPxRigidDynamic->setMass(mass);
    }

    void DynamicPhysicsActorPhysX::AddForce(const glm::vec3& force, const ForceMode& forceMode)
    {
        mPxRigidDynamic->addForce(PhysXConversions::glmVec3ToPxVec3(force), PhysXConversions::forceModeToPxForceMode(forceMode));
    }

    void DynamicPhysicsActorPhysX::AddForceAtPos(const glm::vec3& force, const glm::vec3& pos, const ForceMode& forceMode)
    {
        physx::PxRigidBodyExt::addForceAtPos(*mPxRigidDynamic, PhysXConversions::glmVec3ToPxVec3(force), PhysXConversions::glmVec3ToPxVec3(force), PhysXConversions::forceModeToPxForceMode(forceMode));
    }
}
#endif
