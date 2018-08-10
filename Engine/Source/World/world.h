#ifndef MING3D_WORLD_H
#define MING3D_WORLD_H

#include <vector>

namespace Ming3D
{
    class Actor;
    class Component;

    class World
    {
    private:
        std::vector<Actor*> mActors;

    public:
        void AddActor(Actor* inActor);
        std::vector<Actor*> GetActors() { return mActors; }
    };
}

#endif
