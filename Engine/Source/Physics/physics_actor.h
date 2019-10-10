#ifndef MING3D_PHYSICSACTOR_H
#define MING3D_PHYSICSACTOR_H

#include "Actors/transform.h"

namespace Ming3D
{
    class PhysicsActor
    {
    public:
        virtual void UpdateTransform(const Transform& inTrans) = 0;
    };
}

#endif
