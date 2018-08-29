#include "property.h"

namespace Ming3D
{
    Property::Property(const char* inName, PropertyHandleBase* inPropHandle)
    {
        mName = inName;
        mPropertyHandle = inPropHandle;
    }
}
