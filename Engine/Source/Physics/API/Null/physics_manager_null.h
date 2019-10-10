#ifndef MING3D_PHYSICSMANAGERNULL_H
#define MING3D_PHYSICSMANAGERNULL_H

#include "Physics/physics_manager.h"
#include "physics_scene_null.h"
#include <vector>

namespace Ming3D
{
    class PhysicsManagerNull : public PhysicsManager
    {
    private:
        std::vector<PhysicsSceneNull*> mScenes;

    public:
        PhysicsManagerNull() {};
        virtual ~PhysicsManagerNull() {};
        virtual PhysicsScene* CreatePhysicsScene() override
        {
            PhysicsSceneNull* scene = new PhysicsSceneNull();
            mScenes.push_back(scene);
            return scene;
        };
        
        virtual void SimulateScenes(float seconds) override {};

        virtual PhysicsScene* GetPhysicsScene(int scene) override { return mScenes[scene]; }
    };
}

#endif
