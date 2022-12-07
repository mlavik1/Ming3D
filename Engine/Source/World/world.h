#ifndef MING3D_WORLD_H
#define MING3D_WORLD_H

#include <vector>
#include <type_traits>
#include <string>
#include "SceneRenderer/render_scene.h"
#include <memory>

namespace Ming3D
{
    class Actor;
    class GameEngine;

    class World
    {
    private:
        std::vector<Actor*> mActors;
        GameEngine* mGameEngine;
        std::unique_ptr<RenderScene> mRenderScene;

        void AddActor(Actor* inActor);

    public:
        World(GameEngine* gameEngine);
        ~World();

        void Tick(float inDeltaTime);
        
        template <typename T>
        T* SpawnActor()
        {
            static_assert(std::is_base_of<Actor, T>::value || std::is_same<Actor, T>::value, "Must be a Actor or subclass of Actor");
            T* actor = new T();
            AddActor(actor);
            return actor;
        }
        Actor* SpawnActor();
        Actor* SpawnActor(const std::string& name);

        std::vector<Actor*> GetActors() { return mActors; }
        std::vector<Actor*> GetActorsRecursive();

        GameEngine* GetGameEngine() { return mGameEngine; }
        RenderScene* GetRenderScene() { return mRenderScene.get(); }
    };
}

#endif
