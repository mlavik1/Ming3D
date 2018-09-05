#ifndef MING3D_ACTOR_H
#define MING3D_ACTOR_H

#include "Object/game_object.h"
#include <vector>
#include "transform.h"

namespace Ming3D
{
    class Component;

    class Actor : public GameObject
    {
        DEFINE_CLASS(Ming3D::Actor, Ming3D::GameObject)

    private:
        static void InitialiseClass();
        
        Transform* mTransform;
        std::vector<Component*> mComponents;
        std::vector<Actor*> mChildren;
        bool mIsInitialised = false;

    public:
        Actor();
        virtual ~Actor();

        void AddComponent(Component* inComp);

        template <typename T>
        T* AddComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "Must be a subclass of component");
            T* newComp = new T();
            AddComponent(newComp);
            return newComp;
        }

        virtual void InitialiseActor();
        virtual void Tick(float inDeltaTime);

        void AddChild(Actor* inActor);

        virtual void ReplicateConstruct(DataWriter* outWriter) override;
        virtual void ReceiveReplicateConstruct(DataWriter* inReader) override;

        inline Transform* GetTransform() { return mTransform; }
        std::vector<Component*> GetComponents() { return mComponents; }
    };
}
#endif
