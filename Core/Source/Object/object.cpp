#include "object.h"

IMPLEMENT_CLASS(Ming3D::Object)

namespace Ming3D
{
	Object::Object()
	{

	}

	Object::~Object()
	{

	}

	void Object::CallFunction(Function* inFunc, const FunctionArgs& inArgs)
	{
		inFunc->CallFunction(this, inArgs);
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
