#ifndef MING3D_STATICPHYSICSACTORNULL_H
#define MING3D_STATICPHYSICSACTORNULL_H

#include "Physics/static_physics_actor.h"

namespace Ming3D
{
    class StaticPhysicsActorNull : public StaticPhysicsActor
    {
    public:
        StaticPhysicsActorNull() {};
        virtual ~StaticPhysicsActorNull() {};

        virtual void UpdateTransform(const Transform& inTrans) override {};
    };
}

#endif
