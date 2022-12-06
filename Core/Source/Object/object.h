#ifndef MING3D_OBJECT_H
#define MING3D_OBJECT_H

#include "objdefs.h"

namespace Ming3D
{
	/**
	* Base class for objects.
	* For a class to have reflection support it needs to inherit from this class,
	*  and have DEFINE_CLASS(ClassName, BaseClassName) in the header and IMPLEMENT_CLASS(ClassName) in the source file.
	*/
	class Object
	{
		DEFINE_CLASS_INTERNAL(Object, , 0)

	private:
        ObjectFlag mObjectFlags;
		uint64_t mGuid;

	public:
		Object();
		virtual ~Object();

		/** Calls the specified Function with the given arguments. */
		void CallFunction(Function* inFunc, const FunctionArgs& inArgs);

        virtual void Serialise(DataWriter* outWriter, PropertyFlag inPropFlags = PropertyFlag::Serialise, ObjectFlag inObjFlag = ObjectFlag::Serialise) {}
        virtual void Deserialise(DataWriter* inReader, PropertyFlag inPropFlags = PropertyFlag::Serialise, ObjectFlag inObjFlag = ObjectFlag::Serialise) {}

        virtual void SerialiseProperties(DataWriter* outWriter, PropertyFlag inFlags = (PropertyFlag)0);
        virtual void DeserialiseProperties(DataWriter* inReader, PropertyFlag inFlags = (PropertyFlag)0);

		/**
		* This is where you will register member functions.
		* Derived classes can implement their version of this function to register their member functions.
		*/
		static void InitialiseClass();

        void SetObjectFlag(ObjectFlag inFlag);
        bool HasObjectFlags(ObjectFlag inFlags);

		inline uint64_t GetGuid() { return mGuid; };
	};
}

#endif
