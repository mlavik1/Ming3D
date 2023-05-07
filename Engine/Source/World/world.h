#ifndef MING3D_WORLD_H
#define MING3D_WORLD_H

#include "SceneRenderer/render_scene.h"
#include <memory>
#include <string>
#include <vector>
#include <type_traits>

namespace Ming3D
{
    class Actor;
    class GameEngine;

    class World
    {
    private:
        std::vector<std::shared_ptr<Actor>> mActors;
        GameEngine* mGameEngine;
        std::unique_ptr<RenderScene> mRenderScene;

        void AddActor(std::shared_ptr<Actor> inActor);

    public:
        explicit World(GameEngine* gameEngine);

        void Tick(float inDeltaTime);
        
        template <typename T>
        std::weak_ptr<T> SpawnActor()
        {
            static_assert(std::is_base_of<Actor, T>::value || std::is_same<Actor, T>::value, "Must be a Actor or subclass of Actor");
            std::shared_ptr<T> actor = std::make_shared<T>(this);
            AddActor(actor);
            return actor;
        }
        std::weak_ptr<Actor> SpawnActor();
        std::weak_ptr<Actor> SpawnActor(const std::string& name);

        std::vector<std::weak_ptr<Actor>> GetActors();
        std::vector<std::weak_ptr<Actor>> GetActorsRecursive();

        GameEngine* GetGameEngine() { return mGameEngine; }
        RenderScene* GetRenderScene() { return mRenderScene.get(); }
    };
}

#endif
