#ifndef MING3D_COMPONENT_H
#define MING3D_COMPONENT_H
#include "Object/object.h"

namespace Ming3D
{
    class World;

    class Component : public Object
    {
        DEFINE_CLASS(Ming3D::Component, Ming3D::Object)
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
    };
}
#endif
