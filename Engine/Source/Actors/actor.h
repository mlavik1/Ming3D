#ifndef MING3D_ACTOR_H
#define MING3D_ACTOR_H

#include "Object/game_object.h"
#include "Object/object_ptr.h"
#include "transform.h"
#include "Components/component_callback_types.h"
#include "Components/component.h"
#include <unordered_map>
#include <vector>
#include <memory>

namespace Ming3D
{
    class World;

    class Actor : public GameObject, public std::enable_shared_from_this<Actor> // TODO
    {
        DEFINE_CLASS(Ming3D::Actor, Ming3D::GameObject)

    private:
        static void InitialiseClass();
        
        World* mWorld;
        Transform mTransform;
        std::vector<Component*> mComponents;
        std::string mActorName;

        ActorPtr mParent;
        std::vector<ActorPtr> mChildren;
        std::vector<Component*> newComponents;
        std::unordered_map<ComponentCallbackType, std::vector<Component*>> mCompCallbackSubscribers;

        static uint64_t instanceCounter;

        template<typename T>
        void GetComponentsInChildrenRecursive(std::vector<T*>& comps)
        {
            static_assert(std::is_base_of<Component, T>::value, "Must be a subclass of component");
            std::for_each(mChildren.begin(), mChildren.end(), [&comps](auto& child){ child.lock()->GetComponentsInChildrenRecursive(comps); });
            for(Component* comp : mComponents)
            {
                if (static_cast<Object*>(comp)->GetClass() == T::GetStaticClass())
                    comps.push_back(static_cast<T*>(comp));
            }
        }

        void AddComponent(Component* inComp);

    public:
        Actor() {}

    public:
        explicit Actor(World* world);
        virtual ~Actor();

        // Delete copy constructor and assignment operator
        Actor(const Actor& actor) = delete;
        Actor& operator=(const Actor&) = delete;

        template <typename T>
        T* AddComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "Must be a subclass of component");
            T* newComp = new T();
            AddComponent(newComp);
            return newComp;
        }

        ActorPtr SpawnChildActor();

        void SetParent(Actor* parent);

        World* GetWorld() { return mWorld; }

        virtual void Tick(float inDeltaTime);

        /**
        * Serialises the actor and all its properties, children and components.
        * @param outWriter  The DataWriter used for storing the serialised data.
        * @param inPropFlags  The required property flags of properties to serialise.
        @ param inObjFlags  The object flags of child components and actors to serialise.
        */
        virtual void Serialise(DataWriter* outWriter, PropertyFlag inPropFlags = PropertyFlag::Serialise, ObjectFlag inObjFlags = ObjectFlag::Serialise) override;
        
        /**
        * Deserialises the actor and all its properties, children and components.
        * @param inReader  The DataWriter to read the serialised data from.
        * @param inPropFlags  The required property flags of properties to deserialise.
        @ param inObjFlags  The object flags of child components and actors to deserialise.
        */
        virtual void Deserialise(DataWriter* inReader, PropertyFlag inPropFlags = PropertyFlag::Serialise, ObjectFlag inObjFlags = ObjectFlag::Serialise) override;
        
        void SerialiseComponents(DataWriter* outWriter, PropertyFlag inPropFlags, ObjectFlag inObjFlag);
        void DeserialiseComponents(DataWriter* inReader, PropertyFlag inPropFlags, ObjectFlag inObjFlags);
        void SerialiseChildActors(DataWriter* outWriter, PropertyFlag inPropFlags, ObjectFlag inObjFlags);
        void DeserialiseChildActors(DataWriter* inReader, PropertyFlag inPropFlags, ObjectFlag inObjFlag);

        void RegisterComponentCallback(ComponentCallbackType inType, Component* inComp);

        /** Caled from Transform after movement. */
        void OnTransformMoved();

        void SetActorName(const std::string& name);

        inline Transform& GetTransform() { return mTransform; }
        std::vector<Component*> GetComponents() { return mComponents; }
        std::string GetActorName() { return mActorName; }
        std::vector<ActorPtr> GetChildren();
        
        template<typename T>
        T* GetComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "Must be a subclass of component");
            for(Component* comp : mComponents)
            {
                if (static_cast<Object*>(comp)->GetClass() == T::GetStaticClass())
                    return static_cast<T*>(comp);
            }
            return nullptr;
        }
        
        template<typename T>
        std::vector<T*> GetComponentsInChildren()
        {
            std::vector<T*> comps;
            GetComponentsInChildrenRecursive<T>(comps);
            return comps;
        }
    };
}
#endif
