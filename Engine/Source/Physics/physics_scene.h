#ifndef MING3D_PHYSICSSCENE_H
#define MING3D_PHYSICSSCENE_H

namespace Ming3D
{
    class RigidBodyComponent;

    class PhysicsScene
    {
    public :
        virtual ~PhysicsScene() {}

        virtual void Simulate(float seconds) = 0;

        virtual void RegisterRigidBody(RigidBodyComponent* rbComp) = 0;
        virtual void UnregisterRigidBody(RigidBodyComponent* rbComp) = 0;
    };
}

#endif
