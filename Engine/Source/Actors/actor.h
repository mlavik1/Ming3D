#ifndef MING3D_ACTOR_H
#define MING3D_ACTOR_H

#include "Object/object.h"
#include <vector>
#include "transform.h"

namespace Ming3D
{
    class Component;

    class Actor : public Object
    {
        DEFINE_CLASS(Ming3D::Actor, Ming3D::Object)

    private:
        static void InitialiseClass();
        
        Transform* mTransform;
        std::vector<Component*> mComponents;
        std::vector<Actor*> mChildren;
        bool mIsInitialised = false;

    public:
        Actor();
        virtual ~Actor();

        template <typename T>
        T* AddComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "Must be a subclass of component");
            T* newComp = new T();
            newComp->mParent = this;
            mComponents.push_back(newComp);
            if (mIsInitialised)
            {
                newComp->InitialiseComponent();
            }
            return newComp;
        }

        virtual void InitialiseActor();
        virtual void Tick(float inDeltaTime);

        void AddChild(Actor* inActor);

        inline Transform* GetTransform() { return mTransform; }
        std::vector<Component*> GetComponents() { return mComponents; }
    };
}
#endif
