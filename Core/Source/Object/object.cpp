#include "object.h"

IMPLEMENT_CLASS(Ming3D::Object)

namespace Ming3D
{
	Object::Object()
	{

	}

	Object::~Object()
	{

	}

	void Object::CallFunction(Function* inFunc, const FunctionArgs& inArgs)
	{
		inFunc->CallFunction(this, inArgs);
	}

	void Object::InitialiseClass()
	{
		
	}
}
