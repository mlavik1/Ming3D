#include "static_physics_actor.h"

#include "PxRigidStatic.h"
#include "GameEngine/game_engine.h"
#include "Physics/physics_manager.h"
#include "PxPhysics.h"
#include "physx_conversions.h"

namespace Ming3D
{
    StaticPhysicsActor::StaticPhysicsActor()
    {
        physx::PxTransform trans(physx::PxVec3(0.0f, 0.0f, 0.0f));
        mPxRigidStatic = GGameEngine->GetPhysicsManager()->GetPxPhysics()->createRigidStatic(trans);
    }

    physx::PxRigidActor* StaticPhysicsActor::GetRigidActor()
    {
        return mPxRigidStatic;
    }

    void StaticPhysicsActor::UpdateTransform(const Transform& inTrans)
    {
        physx::PxTransform trans(PhysXConversions::glmVec3ToPxVec3(inTrans.GetWorldPosition()), PhysXConversions::glmQuatToPxQuat(inTrans.GetWorldRotation()));

        mPxRigidStatic->setGlobalPose(trans);
    }
}
