#include "property.h"

namespace Ming3D
{
    Property::Property(const char* inName, PropertyHandleBase* inPropHandle, std::string inTypeName, PropertyFlag inFlags)
    {
        mName = inName;
        mPropertyHandle = inPropHandle;
        mFlags = inFlags;
        mTypeName = inTypeName;
    }

    PropertyFlag Property::GetPropertyFlags()
    {
        return mFlags;
    }

    bool Property::HasPropertyFlag(PropertyFlag inFlag)
    {
        return (mFlags & inFlag) == inFlag;
    }

    std::string Property::GetTypeName()
    {
        return mTypeName;
    }
}
