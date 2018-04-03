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
		DEFINE_CLASS_INTERNAL(Ming3D::Object, , 0)

	private:
		ObjectFlagRegister mObjectFlags;

	public:
		Object();
		virtual ~Object();

		/** Calls the specified Function with the given arguments. */
		void CallFunction(Function* inFunc, const FunctionArgs& inArgs);

		/**
		* This is where you will register member functions.
		* Derived classes can implement their version of this function to register their member functions.
		*/
		static void InitialiseClass();
	};
}

#endif
