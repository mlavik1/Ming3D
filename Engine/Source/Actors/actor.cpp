#include "actor.h"
#include "Source/Components/component.h"
#include <algorithm>
#include <iterator>
#include "World/world.h"

IMPLEMENT_CLASS(Ming3D::Actor)

namespace Ming3D
{
    uint64_t Actor::instanceCounter = 0;

    void Actor::InitialiseClass()
    {
        Actor::GetStaticClass()->RegisterProperty("mActorName", &Actor::mActorName, PropertyFlag::Serialise);
        Actor::GetStaticClass()->RegisterProperty("mTransform", &Actor::mTransform, PropertyFlag::Serialise);
    }

    Actor::Actor(World* world)
    {
        mWorld = world;
        SetObjectFlag(ObjectFlag::Serialise); // serialised by default
        mActorName = std::string("Actor_") + std::to_string(instanceCounter++);
        mTransform.mOnTransformMoved = [this](){ OnTransformMoved(); }; // TODO: Add function for subscribing (multiple subscribers?)
        mCompCallbackSubscribers[ComponentCallbackType::PostMove] = std::vector<Component*>();
    }

    Actor::~Actor()
    {
    }

    void Actor::AddComponent(Component* inComp)
    {
        inComp->mParent = this;
        mComponents.push_back(inComp);
        inComp->InitialiseComponent();
        newComponents.push_back(inComp);
    }

    std::weak_ptr<Actor> Actor::SpawnChildActor()
    {
        std::weak_ptr<Actor> child = GetWorld()->SpawnActor();
        child.lock()->SetParent(weak_from_this()/* TODO */);
        return child;
    }

    // TODO: Handle null parent (need to notify World?)
    void Actor::SetParent(std::weak_ptr<Actor> parent)
    {
        std::shared_ptr<Actor> oldParentPtr = mParent.lock();
        std::shared_ptr<Actor> newParentPtr = parent.lock();
        if (oldParentPtr == newParentPtr)
            return;

        if (oldParentPtr != nullptr)
        {
            oldParentPtr->mChildren.erase(std::remove_if(
                oldParentPtr->mChildren.begin(), oldParentPtr->mChildren.end(),
                [this](const auto& child){ return child.lock().get() == this; }));
            oldParentPtr->mTransform.mChildren.remove(&mTransform);
        }

        mTransform.mParentTransform = &newParentPtr->mTransform;
        newParentPtr->mChildren.push_back(weak_from_this()/* TODO */);
        newParentPtr->mTransform.mChildren.push_back(&mTransform);
        mParent = parent;
        mTransform.UpdateTransformMatrix();
    }

    void Actor::Tick(float inDeltaTime)
    {
        // TODO: do this somewhere else
        for (Component* comp : newComponents)
            comp->InitialTick();
        newComponents.clear();

        // TODO: refactor actor/component update loop
        for (Component* comp : GetComponents())
        {
            comp->Tick(inDeltaTime);
        }

        // TODO: refactor actor/component update loop
        // TODO: World currently ticks *all* actors - only tick root actors, and let them tick their children?
        /*for (Transform* childTrans : mTransform.mChildren)
        {
            childTrans->mActor->Tick(inDeltaTime);
        }*/
    }

    void Actor::Serialise(DataWriter* outWriter, PropertyFlag inPropFlags, ObjectFlag inObjFlag)
    {
        // Replicate properties
        SerialiseProperties(outWriter, inPropFlags);

        // Serialise components
        SerialiseComponents(outWriter, inPropFlags, inObjFlag);

        // Serialise child actors
        SerialiseChildActors(outWriter, inPropFlags, inObjFlag);
    }

    void Actor::Deserialise(DataWriter* inReader, PropertyFlag inPropFlags, ObjectFlag inObjFlags)
    {
        // Deserialise properties
        DeserialiseProperties(inReader, inPropFlags);

        // Construct components and deserialise component properties
        DeserialiseComponents(inReader, inPropFlags, inObjFlags);

        // Construct children and deserialise child actor properties
        DeserialiseChildActors(inReader, inPropFlags, inObjFlags);
    }

    void Actor::SerialiseComponents(DataWriter* outWriter, PropertyFlag inPropFlags, ObjectFlag inObjFlags)
    {
        // Find all components to serialise
        std::vector<Component*> serialisedComponents;
        std::copy_if(
            mComponents.begin(),
            mComponents.end(),
            std::back_inserter(serialisedComponents),
            [inObjFlags] (Component* childComp) { return childComp->HasObjectFlags(inObjFlags); }
        );
    
        // Serialise components
        outWriter->Write(serialisedComponents.size());
        for (Component* childComp : serialisedComponents)
        {
            outWriter->Write(childComp->GetClass()->GetName().size());
            outWriter->Write(childComp->GetClass()->GetName().c_str(), childComp->GetClass()->GetName().size());
            childComp->Serialise(outWriter, inPropFlags, inObjFlags);
        }
    }

    void Actor::DeserialiseComponents(DataWriter* inReader, PropertyFlag inPropFlags, ObjectFlag inObjFlags)
    {
        size_t numComponents = 0;
        inReader->Read(&numComponents, sizeof(size_t));
        for (size_t i = 0; i < numComponents; i++)
        {
            size_t nameLen = 0;
            inReader->Read(&nameLen, sizeof(size_t));
            char* compClassName = new char[nameLen];
            inReader->Read(compClassName, nameLen);
            Class* compClass = Class::GetClassByName(compClassName, false);
            if (compClass == nullptr)
            {
                delete[] compClassName;
                return;
            }
            Component* comp = (Component*)compClass->CreateInstance();
            AddComponent(comp);
            comp->Deserialise(inReader, inPropFlags, inObjFlags);

            delete[] compClassName;
        }
    }

    void Actor::SerialiseChildActors(DataWriter* outWriter, PropertyFlag inPropFlags, ObjectFlag inObjFlags)
    {
        // Find child actors to serialise
        std::vector<Actor*> serialisedChildren;

        for (std::weak_ptr<Actor>& child : mChildren)
        {
            std::shared_ptr<Actor> childActor = child.lock();
            if (childActor->HasObjectFlags(inObjFlags))
                serialisedChildren.push_back(childActor.get());
        }
        // Serialise actors
        outWriter->Write(serialisedChildren.size());
        for (Actor* childActor : serialisedChildren)
        {
            outWriter->Write(childActor->GetClass()->GetName().size());
            outWriter->Write(childActor->GetClass()->GetName().c_str(), childActor->GetClass()->GetName().size() + 1);
            childActor->Serialise(outWriter, inPropFlags, inObjFlags);
        }
    }

    void Actor::DeserialiseChildActors(DataWriter* inReader, PropertyFlag inPropFlags, ObjectFlag inObjFlags)
    {
        size_t numClidren = 0;
        inReader->Read(&numClidren, sizeof(size_t));
        for (size_t i = 0; i < numClidren; i++)
        {
            size_t nameLen = 0;
            inReader->Read(&nameLen, sizeof(size_t));
            char* actorClassName = new char[nameLen];
            inReader->Read(actorClassName, nameLen);
            Class* actorClass = Class::GetClassByName(actorClassName, false);
            if (actorClass == nullptr)
            {
                delete[] actorClassName;
                return;
            }
            // TODO: What do do about this? (Instantiate actor through world somehow? Need to support subclasses of Actor though..)
            Actor* child = (Actor*)actorClass->CreateInstance();
            child->SetParent(weak_from_this() /* TODO */);
            child->Deserialise(inReader, inPropFlags, inObjFlags);

            delete[] actorClassName;
        }
    }

    void Actor::RegisterComponentCallback(ComponentCallbackType inType, Component* inComp)
    {
        auto &compArr = mCompCallbackSubscribers[inType];
        if(std::find(compArr.begin(), compArr.end(), inComp) == compArr.end())
            compArr.push_back(inComp);
    }

    void Actor::OnTransformMoved()
    {
        // Notify components
        auto &subscribers = mCompCallbackSubscribers[ComponentCallbackType::PostMove];
        for (Component* comp : subscribers)
        {
            comp->PostMove();
        }
    }

    void Actor::SetActorName(const std::string& name)
    {
        mActorName = name;
    }

    std::vector<std::weak_ptr<Actor>> Actor::GetChildren()
    {
        return mChildren;
    }
}
