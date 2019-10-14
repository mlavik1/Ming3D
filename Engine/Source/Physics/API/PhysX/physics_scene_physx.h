#ifndef MING3D_PHYSICSSCENEPHYSX_H
#define MING3D_PHYSICSSCENEPHYSX_H

#include "physx_declarations.h"
#include "PxSimulationEventCallback.h"
#include <list>
#include "Physics/physics_scene.h"

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

    class PhysicsScenePhysX : public PhysicsScene
    {
    private:
        physx::PxScene* mPxScene;
        PhysicsSimulationCallback* simulationCallbackHandler;
        std::list<RigidBodyComponent*> mRigidBodies;


    public :
        PhysicsScenePhysX(physx::PxPhysics* pxPhysics);
        virtual ~PhysicsScenePhysX();

        virtual void Simulate(float seconds) override;

        virtual void RegisterRigidBody(RigidBodyComponent* rbComp) override;
        virtual void UnregisterRigidBody(RigidBodyComponent* rbComp) override;

        physx::PxScene* GetPhysXScene() { return mPxScene; }
    };
}

#endif
