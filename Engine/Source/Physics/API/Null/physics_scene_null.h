#ifndef MING3D_PHYSICSSCENENULL_H
#define MING3D_PHYSICSSCENENULL_H

#include "Physics/physics_scene.h"

namespace Ming3D
{
    class PhysicsSceneNull : public PhysicsScene
    {
    public :
        PhysicsSceneNull() {};
        virtual ~PhysicsSceneNull() {};

        virtual void Simulate(float seconds) override {};

        virtual void RegisterRigidBody(RigidBodyComponent* rbComp) override {};
        virtual void UnregisterRigidBody(RigidBodyComponent* rbComp) override {};
    };
}

#endif
