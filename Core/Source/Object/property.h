#ifndef MING3D_PROPERTY_H
#define MING3D_PROPERTY_H

#include "property_reflection.h"
#include <string>

namespace Ming3D
{
    /**
    * Base class for class properties.
    * Supports serialisation.
    */
    class Property
    {
    private:
        std::string mName;
        PropertyHandleBase* mPropertyHandle;

    public:
        Property(const char* inName, PropertyHandleBase* inPropHandle);

        std::string GetPropertyName() { return mName; }
        PropertyHandleBase* GetPropertyHandle() { return mPropertyHandle; }
    };
}

#endif
