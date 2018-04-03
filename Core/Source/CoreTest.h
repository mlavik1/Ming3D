#include "Object/object.h"

#include <string>
#include <vector>

// TEST CLASS
// TODO: Remove later

namespace Ming3D
{
	class TestClass
	{
	public:
		int mTestValue = 1;

		~TestClass()
		{
			
		}
	};

	class CoreTestClass : public Object
	{
		DEFINE_CLASS(Ming3D::CoreTestClass, Ming3D::Object)
	public:
		void IntBoolTestFunction(int a, bool b);

		void StringTestFunction(std::string a);

		void IntVectorTestFunction(std::vector<int> a);

		void IntPointerTestFunction(int* a);

		void ObjectTestFunction(TestClass a);

		static void InitialiseClass();
	};
}
