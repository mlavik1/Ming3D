#include "physics_manager.h"
#include "Debug/debug.h"
#include "PxPhysicsAPI.h"

namespace Ming3D
{
    PhysicsManager::PhysicsManager()
    {
        mDefaultAllocatorCallback = new physx::PxDefaultAllocator();
        mDefaultErrorCallback = new physx::PxDefaultErrorCallback();

        mPxFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, *mDefaultAllocatorCallback, *mDefaultErrorCallback);
        mPxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mPxFoundation, physx::PxTolerancesScale());
        if (mPxPhysics == nullptr)
            LOG_ERROR() << "Error creating PhysX device";
        else
            LOG_INFO() << "Initialised PhysX, version: " << PX_PHYSICS_VERSION_MAJOR << "." << PX_PHYSICS_VERSION_MINOR;
    }

    PhysicsManager::~PhysicsManager()
    {
        delete mDefaultAllocatorCallback;
        delete mDefaultErrorCallback;
        for (PhysicsScene* scene : mScenes)
            delete scene;
        mScenes.clear();
        mPxPhysics->release();
        mPxFoundation->release();
    }

    PhysicsScene* PhysicsManager::CreatePhysicsScene()
    {
        PhysicsScene* scene = new PhysicsScene(mPxPhysics);
        mScenes.push_back(scene);
        return scene;
    }

    void PhysicsManager::SimulateScenes(float seconds)
    {
        for (size_t iScene = 0; iScene < mScenes.size(); iScene++)
        {
            mScenes[iScene]->Simulate(seconds);
        }
    }
}
