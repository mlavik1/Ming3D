#ifndef MING3D_PHYSICSACTOR_H
#define MING3D_PHYSICSACTOR_H

#include "physx_declarations.h"
#include "Actors/transform.h"

namespace Ming3D
{
    class PhysicsActor
    {
    public:
        virtual physx::PxRigidActor* GetRigidActor() = 0;

        virtual void UpdateTransform(const Transform& inTrans) = 0;
    };
}

#endif
