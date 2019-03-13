#include "dynamic_physics_actor.h"

#include "PxRigidDynamic.h"
#include "GameEngine/game_engine.h"
#include "Physics/physics_manager.h"
#include "PxPhysics.h"
#include "Physics/physx_conversions.h"

namespace Ming3D
{
    DynamicPhysicsActor::DynamicPhysicsActor()
    {
        physx::PxTransform trans(physx::PxVec3(0.0f, 0.0f, 0.0f));
        mPxRigidDynamic = GGameEngine->GetPhysicsManager()->GetPxPhysics()->createRigidDynamic(trans);
    }

    physx::PxRigidActor* DynamicPhysicsActor::GetRigidActor()
    {
        return mPxRigidDynamic;
    }

    void DynamicPhysicsActor::UpdateTransform(const Transform& inTrans)
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

    void DynamicPhysicsActor::SetKinematic(bool inKinematic)
    {
        mIsKinematic = inKinematic;
        mPxRigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, inKinematic);
    }
}
