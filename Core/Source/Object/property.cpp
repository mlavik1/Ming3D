#include "property.h"

namespace Ming3D
{
    Property::Property(const char* inName, PropertyHandleBase* inPropHandle, PropertyFlag inFlags)
    {
        mName = inName;
        mPropertyHandle = inPropHandle;
        mFlags = inFlags;
    }

    PropertyFlag Property::GetPropertyFlags()
    {
        return mFlags;
    }

    bool Property::HasPropertyFlag(PropertyFlag inFlag)
    {
        return (mFlags & inFlag) == inFlag;
    }
}
