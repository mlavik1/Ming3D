#include "test_actor.h"
#include "Debug/debug.h"

IMPLEMENT_CLASS(Ming3D::TestActor)

namespace Ming3D
{
    void TestActor::IntBoolTestFunction(int a, bool b)
    {
        LOG_INFO() << "IntBoolTestFunction";
        LOG_INFO() << a;
        LOG_INFO() << b;
    }

    void TestActor::StringTestFunction(std::string a)
    {
        LOG_INFO() << a;
    }

    void TestActor::IntVectorTestFunction(std::vector<int> a)
    {
        for (int val : a)
        {
            LOG_INFO() << val;
        }
    }

    void TestActor::IntPointerTestFunction(int* a)
    {
        LOG_INFO() << a;
    }

    void TestActor::ObjectTestFunction(TestClass a)
    {
        LOG_INFO() << a.mTestValue;
    }

    void TestActor::InitialiseClass()
    {
        // TODO:
        // Register member functions here.
        // Create one function or macro that allows you to easily register member functions.
        // NOTE: Also add support for return values?

        TestActor::GetStaticClass()->RegisterFunction("IntBoolTestFunction", &TestActor::IntBoolTestFunction);
        TestActor::GetStaticClass()->RegisterFunction("StringTestFunction", &TestActor::StringTestFunction);
        TestActor::GetStaticClass()->RegisterFunction("IntVectorTestFunction", &TestActor::IntVectorTestFunction);
        TestActor::GetStaticClass()->RegisterFunction("IntPointerTestFunction", &TestActor::IntPointerTestFunction);
        TestActor::GetStaticClass()->RegisterFunction("ObjectTestFunction", &TestActor::ObjectTestFunction);
    }
}
