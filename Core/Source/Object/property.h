#ifndef MING3D_PROPERTY_H
#define MING3D_PROPERTY_H

#include "property_reflection.h"
#include <string>
#include <stdint.h>

namespace Ming3D
{
    typedef uint64_t propflag_t;
    enum class PropertyFlag
    {
        Replicated = 1
    };
    
    /**
    * Base class for class properties.
    * Supports serialisation.
    */
    class Property
    {
    private:
        std::string mName;
        PropertyHandleBase* mPropertyHandle;
        propflag_t mFlags = 0;

    public:
        Property(const char* inName, PropertyHandleBase* inPropHandle, propflag_t inFlags = 0);

        std::string GetPropertyName() { return mName; }
        PropertyHandleBase* GetPropertyHandle() { return mPropertyHandle; }

        propflag_t GetPropertyFlags();
        bool HasPropertyFlag(PropertyFlag inFlag);
    };
}

#endif
