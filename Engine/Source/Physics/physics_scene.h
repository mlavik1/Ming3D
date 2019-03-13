#ifndef MING3D_PHYSICSSCENE_H
#define MING3D_PHYSICSSCENE_H

#include "physx_declarations.h"
#include "PxSimulationEventCallback.h"
#include <list>

namespace Ming3D
{
    class RigidBodyComponent;

    class PhysicsSimulationCallback : public physx::PxSimulationEventCallback
    {
    public:
        virtual void onConstraintBreak(physx::PxConstraintInfo * constraints, physx::PxU32 count) override;
        virtual void onWake(physx::PxActor ** actors, physx::PxU32 count) override;
        virtual void onSleep(physx::PxActor ** actors, physx::PxU32 count) override;
        virtual void onContact(const physx::PxContactPairHeader & pairHeader, const physx::PxContactPair * pairs, physx::PxU32 nbPairs) override;
        virtual void onTrigger(physx::PxTriggerPair * pairs, physx::PxU32 count) override;
        virtual void onAdvance(const physx::PxRigidBody * const * bodyBuffer, const physx::PxTransform * poseBuffer, const physx::PxU32 count) override;
    };

    class PhysicsScene
    {
    private:
        physx::PxScene* mPxScene;
        PhysicsSimulationCallback* simulationCallbackHandler;
        std::list<RigidBodyComponent*> mRigidBodies;


    public :
        PhysicsScene(physx::PxPhysics* pxPhysics);
        ~PhysicsScene();

        void Simulate(float seconds);

        void RegisterRigidBody(RigidBodyComponent* rbComp);
        void UnregisterRigidBody(RigidBodyComponent* rbComp);

        physx::PxScene* GetPhysXScene() { return mPxScene; }
    };
}

#endif
