#ifndef MING3D_DYNAMICPHYSICSACTOR_H
#define MING3D_DYNAMICPHYSICSACTOR_H

#include "physics_actor.h"
#include "Actors/transform.h"

namespace Ming3D
{
    class DynamicPhysicsActor : public PhysicsActor
    {
    private:
        physx::PxRigidDynamic* mPxRigidDynamic = nullptr;

        bool mIsKinematic = false;

    public:
        DynamicPhysicsActor();
        virtual physx::PxRigidActor* GetRigidActor() override;

        virtual void UpdateTransform(const Transform& inTrans) override;

        void SetKinematic(bool inKinematic);

        bool IsKinematic() const { return mIsKinematic; };
    };
}

#endif
