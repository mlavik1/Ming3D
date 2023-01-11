#ifdef MING3D_PHYSX
#include "physics_scene_physx.h"

#include "PxScene.h"
#include "PxPhysics.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "Components/rigidbody_component.h"
#include "PxRigidActor.h"
#include "foundation/PxTransform.h"
#include "Actors/actor.h"
#include "physx_conversions.h"
#include "iphysx_actor.h"

namespace Ming3D
{
    void PhysicsSimulationCallback::onConstraintBreak(physx::PxConstraintInfo * constraints, physx::PxU32 count)
    {

    }

    void PhysicsSimulationCallback::onWake(physx::PxActor ** actors, physx::PxU32 count)
    {

    }

    void PhysicsSimulationCallback::onSleep(physx::PxActor ** actors, physx::PxU32 count)
    {

    }

    void PhysicsSimulationCallback::onContact(const physx::PxContactPairHeader & pairHeader, const physx::PxContactPair * pairs, physx::PxU32 nbPairs)
    {

    }

    void PhysicsSimulationCallback::onTrigger(physx::PxTriggerPair * pairs, physx::PxU32 count)
    {

    }

    void PhysicsSimulationCallback::onAdvance(const physx::PxRigidBody * const * bodyBuffer, const physx::PxTransform * poseBuffer, const physx::PxU32 count)
    {

    }

    static physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags &pairFlags, const void *constBlock, physx::PxU32 constantBlockSize)
    {
        pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
            | physx::PxPairFlag::eTRIGGER_DEFAULT
            //| physx::PxPairFlag::eCCD_LINEAR; // CCD
            | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
            | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;


        return physx::PxFilterFlag::eDEFAULT;
    }

    PhysicsScenePhysX::PhysicsScenePhysX(physx::PxPhysics* pxPhysics)
    {
        simulationCallbackHandler = new PhysicsSimulationCallback();

        physx::PxSceneDesc sceneDesc(pxPhysics->getTolerancesScale());
        sceneDesc.gravity = physx::PxVec3(0.0f, -9.8f, 0.0f);
        sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
        sceneDesc.filterShader = contactReportFilterShader;
        sceneDesc.simulationEventCallback = simulationCallbackHandler;
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;

        mPxScene = pxPhysics->createScene(sceneDesc);

        mPxScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0);	//Global visualization scale which gets multiplied with the individual scales
        mPxScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);	//Enable visualization of actor's shape
        mPxScene->setVisualizationParameter(physx::PxVisualizationParameter::eACTOR_AXES, 1.0f);	//Enable visualization of actor's axis
    }

    PhysicsScenePhysX::~PhysicsScenePhysX()
    {
        mPxScene->release();
        mPxScene = nullptr;
    }

    void PhysicsScenePhysX::Simulate(float seconds)
    {
        physx::PxReal timestep = static_cast<physx::PxReal>(seconds);
        mPxScene->simulate(timestep);
        mPxScene->fetchResults(true);

        for (std::list<RigidBodyComponent*>::iterator iter = mRigidBodies.begin(); iter != mRigidBodies.end(); ++iter)
        {
            RigidBodyComponent* rbComp = *iter;
            if (rbComp->IsStatic())
                continue;

            physx::PxTransform pose = dynamic_cast<IPhysXActor*>(rbComp->GetPhysicsActor())->GetRigidActor()->getGlobalPose();
            rbComp->GetParent()->GetTransform().SetWorldPosition(PhysXConversions::pxVec3ToGlmVec3(pose.p));
            rbComp->GetParent()->GetTransform().SetWorldRotation(PhysXConversions::pxQuatToGlmQuat(pose.q));
        }
    }

    void PhysicsScenePhysX::RegisterRigidBody(RigidBodyComponent* rbComp)
    {
        mPxScene->addActor(*dynamic_cast<IPhysXActor*>(rbComp->GetPhysicsActor())->GetRigidActor());
        mRigidBodies.push_back(rbComp);
    }

    void PhysicsScenePhysX::UnregisterRigidBody(RigidBodyComponent* rbComp)
    {
        mPxScene->removeActor(*dynamic_cast<IPhysXActor*>(rbComp->GetPhysicsActor())->GetRigidActor());
        mRigidBodies.remove(rbComp);
    }
}
#endif
