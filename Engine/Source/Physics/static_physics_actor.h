#ifndef MING3D_STATICPHYSICSACTOR_H
#define MING3D_STATICPHYSICSACTOR_H

#include "physics_actor.h"

namespace Ming3D
{
    class StaticPhysicsActor : public PhysicsActor
    {
    private:
        physx::PxRigidStatic* mPxRigidStatic = nullptr;

    public:
        StaticPhysicsActor();

        virtual physx::PxRigidActor* GetRigidActor() override;

        virtual void UpdateTransform(const Transform& inTrans) override;
    };
}

#endif
