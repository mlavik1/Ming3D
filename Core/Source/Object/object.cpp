#include "object.h"
#include "object_ref_handle.h"

IMPLEMENT_CLASS(Ming3D::Object)

namespace Ming3D
{
    Object::Object()
    : mObjectFlags(ObjectFlag::None)
    {
        static uint64_t GUID = 0;
        mGuid = GUID++;
        mObjectRefHandle = new ObjectRefHandle(this);
    }

    Object::~Object()
    {
        mObjectRefHandle->ReleaseObject();
    }

    void Object::CallFunction(Function* inFunc, const FunctionArgs& inArgs)
    {
        inFunc->CallFunction(this, inArgs);
    }

    void Object::Serialise(DataWriter*, PropertyFlag, ObjectFlag)
    {
    }
    
    void Object::Deserialise(DataWriter*, PropertyFlag, ObjectFlag)
    {
    }

    void Object::SerialiseProperties(DataWriter* outWriter, PropertyFlag inFlags)
    {
        for (Property* prop : GetClass()->GetAllProperties(true))
        {
            if (prop->HasPropertyFlag(inFlags))
                prop->GetPropertyHandle()->Serialise(this, *outWriter);
        }
    }

    void Object::DeserialiseProperties(DataWriter* inReader, PropertyFlag inFlags )
    {
        for (Property* prop : GetClass()->GetAllProperties(true))
        {
            if (prop->HasPropertyFlag(inFlags))
                prop->GetPropertyHandle()->Deserialise(this, *inReader);
        }
    }

    void Object::InitialiseClass()
    {
        
    }

    void Object::SetObjectFlag(ObjectFlag inFlag)
    {
        mObjectFlags = mObjectFlags | inFlag;
    }

    bool Object::HasObjectFlags(ObjectFlag inFlags)
    {
        return (mObjectFlags & inFlags) != (ObjectFlag)0;
    }
}
