#include "CoreTest.h"
#include "Debug/debug.h"

IMPLEMENT_CLASS(Ming3D::CoreTestClass)

namespace Ming3D
{
	void CoreTestClass::IntBoolTestFunction(int a, bool b)
	{
		LOG_INFO() << a;
		LOG_INFO() << b;
	}

	void CoreTestClass::StringTestFunction(std::string a)
	{
		LOG_INFO() << a;
	}

	void CoreTestClass::IntVectorTestFunction(std::vector<int> a)
	{
		for (int val : a)
		{
			LOG_INFO() << val;
		}
	}

	void CoreTestClass::IntPointerTestFunction(int* a)
	{
		LOG_INFO() << a;
	}

	void CoreTestClass::ObjectTestFunction(TestClass a)
	{
		LOG_INFO() << a.mTestValue;
	}

	void CoreTestClass::InitialiseClass()
	{
		// TODO:
		// Register member functions here.
		// Create one function or macro that allows you to easily register member functions.
		// NOTE: Also add support for return values?

		CoreTestClass::GetStaticClass()->RegisterFunction("IntBoolTestFunction", &CoreTestClass::IntBoolTestFunction);
		CoreTestClass::GetStaticClass()->RegisterFunction("StringTestFunction", &CoreTestClass::StringTestFunction);
		CoreTestClass::GetStaticClass()->RegisterFunction("IntVectorTestFunction", &CoreTestClass::IntVectorTestFunction);
		CoreTestClass::GetStaticClass()->RegisterFunction("IntPointerTestFunction", &CoreTestClass::IntPointerTestFunction);
		CoreTestClass::GetStaticClass()->RegisterFunction("ObjectTestFunction", &CoreTestClass::ObjectTestFunction);
	}
}
