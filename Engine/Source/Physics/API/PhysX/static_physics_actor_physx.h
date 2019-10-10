#ifndef MING3D_STATICPHYSICSACTORPHYSX_H
#define MING3D_STATICPHYSICSACTORPHYSX_H

#include "Physics/static_physics_actor.h"
#include "iphysx_actor.h"

namespace Ming3D
{
    class StaticPhysicsActorPhysX : public StaticPhysicsActor, public IPhysXActor
    {
    private:
        physx::PxRigidStatic* mPxRigidStatic = nullptr;

    public:
        StaticPhysicsActorPhysX();
        virtual ~StaticPhysicsActorPhysX() {};

        virtual physx::PxRigidActor* GetRigidActor() override;

        virtual void UpdateTransform(const Transform& inTrans) override;
    };
}

#endif
