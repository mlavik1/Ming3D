#include "game_object.h"

IMPLEMENT_CLASS(Ming3D::GameObject)

namespace Ming3D
{
    void GameObject::InitialiseClass()
    {

    }

    void GameObject::Serialise(DataWriter*, PropertyFlag, ObjectFlag)
    {
    }

    void GameObject::Deserialise(DataWriter*, PropertyFlag, ObjectFlag)
    {
    }

    void GameObject::ReplicateConstruct(DataWriter*)
    {
    }

    void GameObject::ReceiveReplicateConstruct(DataWriter*)
    {
    }
}
