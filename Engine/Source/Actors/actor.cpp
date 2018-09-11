#include "actor.h"
#include "Source/Components/component.h"

IMPLEMENT_CLASS(Ming3D::Actor)

namespace Ming3D
{
    void Actor::InitialiseClass()
    {

    }

    Actor::Actor()
    {
        mTransform = new Transform();
        mTransform->mActor = this;
    }

    Actor::~Actor()
    {
        delete mTransform;
    }

    void Actor::AddComponent(Component* inComp)
    {
        inComp->mParent = this;
        mComponents.push_back(inComp);
        if (mIsInitialised)
        {
            inComp->InitialiseComponent();
        }
    }

    void Actor::InitialiseActor()
    {
        mIsInitialised = true;
    }

    void Actor::Tick(float inDeltaTime)
    {

    }

    void Actor::AddChild(Actor* inActor)
    {
        mChildren.push_back(inActor);
        inActor->mTransform->mParentTransform = mTransform;
    }

    void Actor::ReplicateConstruct(DataWriter* outWriter)
    {
        // Replicate properties
        for (Property* prop : GetClass()->GetAllProperties(true))
        {
            if(prop->HasPropertyFlag(PropertyFlag::Replicated))
                prop->GetPropertyHandle()->Serialise(this, *outWriter);
        }

        // Replicate components
        outWriter->Write(mComponents.size());
        for (Component* childComp : mComponents)
        {
            outWriter->Write(childComp->GetClass()->GetName().size());
            outWriter->Write(childComp->GetClass()->GetName().c_str(), childComp->GetClass()->GetName().size()); // TODO: template function
            childComp->ReplicateConstruct(outWriter);
        }

        // Replicate child actors
        outWriter->Write(mChildren.size());
        for (Actor* childActor : mChildren)
        {
            outWriter->Write(childActor->GetClass()->GetName().size());
            outWriter->Write(childActor->GetClass()->GetName().c_str(), childActor->GetClass()->GetName().size() + 1); // TODO: template function
            childActor->ReplicateConstruct(outWriter);
        }
    }

    void Actor::ReceiveReplicateConstruct(DataWriter* inReader)
    {
        // Deserialise properties
        for (Property* prop : GetClass()->GetAllProperties(true))
        {
            if (prop->HasPropertyFlag(PropertyFlag::Replicated))
                prop->GetPropertyHandle()->Deserialise(this, *inReader);
        }

        // Construct components and deserialise component properties
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
            comp->ReceiveReplicateConstruct(inReader);

            delete[] compClassName;
        }

        // Construct children and deserialise child actor properties
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
            Actor* child = (Actor*)actorClass->CreateInstance();
            AddChild(child);
            child->ReceiveReplicateConstruct(inReader);

            delete[] actorClassName;
        }
    }
}
