#ifdef MING3D_PHYSX
#include "physics_manager_physx.h"
#include "Debug/debug.h"
#include "PxPhysicsAPI.h"
#include "physics_scene_physx.h"

namespace Ming3D
{
    PhysicsManagerPhysX::PhysicsManagerPhysX()
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

    PhysicsManagerPhysX::~PhysicsManagerPhysX()
    {
        delete mDefaultAllocatorCallback;
        delete mDefaultErrorCallback;
        for (PhysicsScene* scene : mScenes)
            delete scene;
        mScenes.clear();
        mPxPhysics->release();
        mPxFoundation->release();
    }

    PhysicsScene* PhysicsManagerPhysX::CreatePhysicsScene()
    {
        PhysicsScenePhysX* scene = new PhysicsScenePhysX(mPxPhysics);
        mScenes.push_back(scene);
        return scene;
    }

    void PhysicsManagerPhysX::SimulateScenes(float seconds)
    {
        for (size_t iScene = 0; iScene < mScenes.size(); iScene++)
        {
            mScenes[iScene]->Simulate(seconds);
        }
    }
}
#endif
