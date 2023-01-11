#ifndef MING3D_GAMEOBJECT_H
#define MING3D_GAMEOBJECT_H

#include "object.h"
#include "objdefs.h"

namespace Ming3D
{
    /**
    * Base calss of Actors and Components (Objects in a scene).
    */
    class GameObject : public Object
    {
        DEFINE_CLASS(Ming3D::GameObject, Ming3D::Object)

    private:
        static void InitialiseClass();

    public:
        netguid_t mNetGUID = 0;

        virtual void Serialise(DataWriter* outWriter, PropertyFlag inPropFlags = PropertyFlag::Serialise, ObjectFlag inObjFlag = ObjectFlag::Serialise) override;
        virtual void Deserialise(DataWriter* inReader, PropertyFlag inPropFlags = PropertyFlag::Serialise, ObjectFlag inObjFlag = ObjectFlag::Serialise) override;
        virtual void ReplicateConstruct(DataWriter* outWriter);
        virtual void ReceiveReplicateConstruct(DataWriter* inReader);

    };
}

#endif
