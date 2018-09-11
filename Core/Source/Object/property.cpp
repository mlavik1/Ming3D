#include "property.h"

namespace Ming3D
{
    Property::Property(const char* inName, PropertyHandleBase* inPropHandle, propflag_t inFlags)
    {
        mName = inName;
        mPropertyHandle = inPropHandle;
        mFlags = inFlags;
    }

    propflag_t Property::GetPropertyFlags()
    {
        return mFlags;
    }

    bool Property::HasPropertyFlag(PropertyFlag inFlag)
    {
        return mFlags & (propflag_t)inFlag != 0;
    }
}
