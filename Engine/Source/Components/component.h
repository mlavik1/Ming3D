#ifndef MING3D_COMPONENT_H
#define MING3D_COMPONENT_H

#include "Object/game_object.h"

namespace Ming3D
{
    class World;

    class Component : public GameObject
    {
        DEFINE_CLASS(Ming3D::Component, Ming3D::GameObject)
            friend class Actor;

    private:
        static void InitialiseClass();

    protected:
        Actor* mParent = nullptr;

    public:
        Component();
        virtual ~Component();

        virtual void InitialiseComponent();
        virtual void Tick(float inDeltaTime);

        Actor* GetParent() { return mParent; }
    };
}
#endif
