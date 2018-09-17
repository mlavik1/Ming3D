#ifndef MING3D_PROPERTY_H
#define MING3D_PROPERTY_H

#include "property_reflection.h"
#include <string>
#include <stdint.h>

namespace Ming3D
{
    enum class PropertyFlag
    {
        InitReplicate = 1,
        Serialise = 2
    };

    inline PropertyFlag operator|(PropertyFlag a, PropertyFlag b)
    {
        return static_cast<PropertyFlag>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline PropertyFlag operator&(PropertyFlag a, PropertyFlag b)
    {
        return static_cast<PropertyFlag>(static_cast<int>(a) & static_cast<int>(b));
    }
    
    /**
    * Base class for class properties.
    * Supports serialisation.
    */
    class Property
    {
    private:
        std::string mName;
        PropertyHandleBase* mPropertyHandle;
        PropertyFlag mFlags = (PropertyFlag)0;

    public:
        Property(const char* inName, PropertyHandleBase* inPropHandle, PropertyFlag inFlags = (PropertyFlag)0);

        std::string GetPropertyName() { return mName; }
        PropertyHandleBase* GetPropertyHandle() { return mPropertyHandle; }

        PropertyFlag GetPropertyFlags();
        bool HasPropertyFlag(PropertyFlag inFlag);

    };
}

#endif
