#ifndef MING3D_WORLD_H
#define MING3D_WORLD_H

#include "SceneRenderer/render_scene.h"
#include "Object/object_ptr.h"
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
        std::vector<std::unique_ptr<Actor>> mActors;
        GameEngine* mGameEngine;
        std::unique_ptr<RenderScene> mRenderScene;

        void InitialiseActor(Actor* actor);

    public:
        explicit World(GameEngine* gameEngine);

        void Tick(float inDeltaTime);
        
        template <typename T>
        ObjectPtr<T> SpawnActor()
        {
            static_assert(std::is_base_of<Actor, T>::value || std::is_same<Actor, T>::value, "Must be a Actor or subclass of Actor");
            std::unique_ptr<T> actor = std::make_unique<T>(this);
            ObjectPtr<T> ptr = actor.get();
            InitialiseActor(ptr.Get());
            mActors.push_back(std::move(actor));
            return ptr;
        }
        ActorPtr SpawnActor();
        ActorPtr SpawnActor(const std::string& name);

        std::vector<ActorPtr> GetActors();
        std::vector<ActorPtr> GetActorsRecursive();

        GameEngine* GetGameEngine() { return mGameEngine; }
        RenderScene* GetRenderScene() { return mRenderScene.get(); }
    };
}

#endif
