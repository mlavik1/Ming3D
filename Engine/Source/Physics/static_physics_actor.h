#ifndef MING3D_STATICPHYSICSACTOR_H
#define MING3D_STATICPHYSICSACTOR_H

#include "physics_actor.h"

namespace Ming3D
{
    class StaticPhysicsActor : public PhysicsActor
    {
    public:
        StaticPhysicsActor() {};
        virtual ~StaticPhysicsActor() override = default;

        virtual void UpdateTransform(const Transform& inTrans) override = 0;
    };
}

#endif
