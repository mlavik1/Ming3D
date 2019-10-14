#ifndef MING3D_IPHYSXACTOR_H
#define MING3D_IPHYSXACTOR_H

#include "physx_declarations.h"

namespace Ming3D
{
    class IPhysXActor
    {
    public:
        virtual physx::PxRigidActor* GetRigidActor() = 0;
    };
}

#endif
