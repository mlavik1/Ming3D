#if MING3D_TESTTYPE == 1
#include "test_actor.h"

#include "GameEngine/game_engine.h"
#include "Debug/debug.h"

using namespace Ming3D;

int main()
{
    GameEngine engine;
    engine.Initialise(); // in order to register classes

    Actor* camActor = engine->GetWorld()->SpawnActor();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));

    PropertyHandleBase* propHandle = TestActor::GetStaticClass()->GetPropertyByName("TestPropertyInt")->GetPropertyHandle();

    TestActor* actor1 = new TestActor();
    actor1->TestPropertyInt = 99;

    DataWriter dw1(10);
    propHandle->Serialise(actor1, dw1);

    TestActor* actor2 = new TestActor();
    propHandle->Deserialise(actor2, dw1);

    LOG_INFO() << actor2->TestPropertyInt;

    std::vector<int> intVec({ 1, 3, 7 });
    int* intPtr = new int(3);

    Function* funcTestFunction = TestActor::GetStaticClass()->GetFunctionByName("IntBoolTestFunction");
    Function* funcStringTestFunction = TestActor::GetStaticClass()->GetFunctionByName("StringTestFunction");
    Function* funcIntVectorTestFunction = TestActor::GetStaticClass()->GetFunctionByName("IntVectorTestFunction");
    Function* funcIntPointerTestFunction = TestActor::GetStaticClass()->GetFunctionByName("IntPointerTestFunction");
    Function* funcObjectTestFunction = TestActor::GetStaticClass()->GetFunctionByName("ObjectTestFunction");

    TestActor* testMingObject = new TestActor();

    testMingObject->CallFunction(funcTestFunction, FunctionArgs({ FunctionParam<int>(3), FunctionParam<bool>(true) }));
    testMingObject->CallFunction(funcStringTestFunction, FunctionArgs({ FunctionParam<std::string>("test") }));
    testMingObject->CallFunction(funcIntVectorTestFunction, FunctionArgs({ FunctionParam<std::vector<int>>(intVec) }));
    testMingObject->CallFunction(funcIntPointerTestFunction, FunctionArgs({ FunctionParam<int*>(intPtr) }));

    FunctionArgs intVecArgs1({ FunctionParam<std::vector<int>>(intVec) });
    DataWriter dataWriter(1);
    funcIntVectorTestFunction->SerialiseFunctionArgs(intVecArgs1, dataWriter);
    FunctionArgs intVecArgs2 = funcIntVectorTestFunction->DeserialiseFunctionArgs(dataWriter);

    testMingObject->CallFunction(funcIntVectorTestFunction, intVecArgs2);

    TestClass testClass;
    testClass.mTestValue = 3;
    testMingObject->CallFunction(funcObjectTestFunction, FunctionArgs({ FunctionParam<TestClass>(testClass) }));


    while (true)
    {

    }

    return 0;
}

#endif
