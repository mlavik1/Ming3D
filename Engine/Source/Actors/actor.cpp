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

    ActorPtr Actor::SpawnChildActor()
    {
        ActorPtr child = GetWorld()->SpawnActor();
        child->SetParent(this);
        return child;
    }

    // TODO: Handle null parent (need to notify World?)
    void Actor::SetParent(Actor* newParent)
    {
        if (mParent == newParent)
            return;

        if (mParent != nullptr)
        {
            mParent->mChildren.erase(std::remove_if(
                mParent->mChildren.begin(), mParent->mChildren.end(),
                [this](const auto& child){ return child == this; }));
            mParent->mTransform.mChildren.remove(&mTransform);
        }

        mTransform.mParentTransform = &newParent->mTransform;
        newParent->mChildren.push_back(this);
        newParent->mTransform.mChildren.push_back(&mTransform);
        mParent = newParent;
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

        for (ActorPtr& child : mChildren)
        {
            if (child->HasObjectFlags(inObjFlags))
                serialisedChildren.push_back(child.Get());
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
            Actor* child = static_cast<Actor*>(actorClass->CreateInstance());
            child->SetParent(this);
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

    std::vector<ActorPtr> Actor::GetChildren()
    {
        return mChildren;
    }
}
