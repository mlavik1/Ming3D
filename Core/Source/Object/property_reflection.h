#ifndef MING3D_PROPERTYREFLECTION_H
#define MING3D_PROPERTYREFLECTION_H

#include "Serialisation/data_writer.h"
#include "Serialisation/data_serialisation.h"

namespace Ming3D
{
    /**
    * Handle for property reflections.
    */
    class PropertyHandleBase
    {
    public:
        virtual void Serialise(void* inObject, DataWriter& outDataWriter) = 0;
        virtual void Deserialise(void* outObject, DataWriter& inDataWriter) = 0;
        virtual void* GetValuePtr(void* inObject) = 0;
    };

    template<typename VarType, typename ClassType>
    class PropertyHandle : public PropertyHandleBase
    {
    public:
        /** Member variable pointer */
        VarType ClassType::*varPtr;

        PropertyHandle(VarType ClassType::*inVarPtr)
        {
            varPtr = inVarPtr;
        }

        virtual void Serialise(void* inObject, DataWriter& outDataWriter) override
        {
            TypeSerialisationTraits<VarType>::Write(outDataWriter, ((ClassType*)inObject)->*varPtr);

        }
        virtual void Deserialise(void* outObject, DataWriter& inDataWriter) override
        {
            TypeSerialisationTraits<VarType>::Read(inDataWriter, ((ClassType*)outObject)->*varPtr);
        }
        virtual void* GetValuePtr(void* inObject) override
        {
            return &(((ClassType*)inObject)->*varPtr);
        }
    };
}

#endif
