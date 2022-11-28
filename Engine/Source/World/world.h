#ifndef MING3D_WORLD_H
#define MING3D_WORLD_H

#include <vector>
#include <type_traits>

namespace Ming3D
{
    class Actor;
    class GameEngine;

    class World
    {
    private:
        std::vector<Actor*> mActors;
        GameEngine* mGameEngine;

        void AddActor(Actor* inActor);

    public:
        World(GameEngine* gameEngine);
        ~World();

        template <typename T>
        T* SpawnActor()
        {
            static_assert(std::is_base_of<Actor, T>::value || std::is_same<Actor, T>::value, "Must be a Actor or subclass of Actor");
            T* actor = new T();
            AddActor(actor);
            return actor;
        }
        Actor* SpawnActor();

        std::vector<Actor*> GetActors() { return mActors; }

        GameEngine* GetGameEngine() { return mGameEngine; }
    };
}

#endif
