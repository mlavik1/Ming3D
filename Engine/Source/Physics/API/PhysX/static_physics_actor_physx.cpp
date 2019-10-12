#ifdef MING3D_PHYSX
#include "static_physics_actor_physx.h"

#include "PxRigidStatic.h"
#include "GameEngine/game_engine.h"
#include "physics_manager_physx.h"
#include "PxPhysics.h"
#include "physx_conversions.h"

namespace Ming3D
{
    StaticPhysicsActorPhysX::StaticPhysicsActorPhysX()
    {
        physx::PxTransform trans(physx::PxVec3(0.0f, 0.0f, 0.0f));
        mPxRigidStatic = static_cast<PhysicsManagerPhysX*>(GGameEngine->GetPhysicsManager())->GetPxPhysics()->createRigidStatic(trans);
    }

    physx::PxRigidActor* StaticPhysicsActorPhysX::GetRigidActor()
    {
        return mPxRigidStatic;
    }

    void StaticPhysicsActorPhysX::UpdateTransform(const Transform& inTrans)
    {
        physx::PxTransform trans(PhysXConversions::glmVec3ToPxVec3(inTrans.GetWorldPosition()), PhysXConversions::glmQuatToPxQuat(inTrans.GetWorldRotation()));

        mPxRigidStatic->setGlobalPose(trans);
    }
}
#endif
