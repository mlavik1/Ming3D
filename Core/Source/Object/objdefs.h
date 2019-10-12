#ifndef MING3D_OBJDEFS_H
#define MING3D_OBJDEFS_H

#include <stdint.h>
#include "class.h"
#include "function.h"

/*=============================================================================================
Macros and more for Ming3D::Object and subclasses.
Enabling reflection on a class:
	class YourClass : public Object
	{
	DEFINE_CLASS(YourClass, Object)
	...
	}
Registering a function (so you can call it by name or use RPCs):
	DEFINE_FUNCTION(FunctionName, FunctionParameter1Type, FunctionParameter2Type, ......)
	BEGIN_REGISTER_CLASSPROPERTIES(Ming3D::YourClass)
	REGISTER_CLASS_FUNCTION(Ming3D::YourClass, FunctionName) // REPEAT FOR EACH FUNCTION
	.....
	END_REGISTER_CLASSPROPERTIES(Ming3D::YourClass)
==============================================================================================*/

typedef uint64_t netguid_t;

enum class ObjectFlag
{
	Destroyed = 1,
	InitReplicate = 2,
    Serialise = 4
};

inline ObjectFlag operator|(ObjectFlag a, ObjectFlag b)
{
    return static_cast<ObjectFlag>(static_cast<int>(a) | static_cast<int>(b));
}

inline ObjectFlag operator&(ObjectFlag a, ObjectFlag b)
{
    return static_cast<ObjectFlag>(static_cast<int>(a) & static_cast<int>(b));
}

// Used in the constructor defined in DEFINE_CLASS_INTERNAL.
// This allows us to create an instance of a class without knowing the calss ( see Class::CreateInstance() ).
struct MingObject_EmptyConstructorParams
{

};

struct ObjectInitialiserParams
{

};

// Do not use this directly!
#define DEFINE_CLASS_INTERNAL(name, constructorinitlist, baseclassaccessor) \
private: \
	static Ming3D::Class* StaticClass; \
	static bool ClassPropertiesRegistered; \
public: \
	static Ming3D::Class* GetStaticClass() \
	{ \
		if(StaticClass == nullptr) StaticClass = name ::CreateStaticClass(); \
		return StaticClass; \
	} \
	static Ming3D::Class* CreateStaticClass() \
	{ \
		if(StaticClass) return StaticClass; \
		return new Ming3D::Class(#name, & name ::InitialiseClass, & name ::_CreateInstanceFromDefaultConstructor, baseclassaccessor); \
	} \
	virtual Ming3D::Class* GetClass() \
	{ \
		return GetStaticClass(); \
	} \
	\
	static Ming3D::Object* _CreateInstanceFromDefaultConstructor() \
	{ \
		return new name (); \
	}

/**
* Define a class, that directly or indirectly inherits from Ming3D::Object.
*
* The class will have Run-Time Type Information available through GetClass() and GetStaticClass().
* This macro will generate a constructor with parameters, so the class will have no implicit default constructor.
* IMPORTANT: You will need to add the IMPLEMENT_CLASS(classname)-macro to your source file.
*/
#define DEFINE_CLASS(classname, baseclassname) \
    typedef baseclassname Super; \
	DEFINE_CLASS_INTERNAL(classname, : baseclassname (params), baseclassname ::GetStaticClass())

/**
* Add this to your source file, if DEFINE_CLASS has been used in the header file.
*/
#define IMPLEMENT_CLASS(classname) \
	Ming3D::Class* classname ::StaticClass = classname ::CreateStaticClass();

#endif
