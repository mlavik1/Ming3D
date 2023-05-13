#if MING3D_TESTTYPE == 1
#include "test_actor.h"

#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "Debug/debug.h"
#include "Components/camera_component.h"

using namespace Ming3D;

int main()
{
    GameEngine engine;
    engine.Initialise(); // in order to register classes

    std::shared_ptr<Actor> camActor = engine.GetWorld().lock()->SpawnActor().lock();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));

    PropertyHandleBase* propHandle = TestActor::GetStaticClass()->GetPropertyByName("TestPropertyInt")->GetPropertyHandle();

    std::shared_ptr<TestActor> actor1 = engine.GetWorld().lock()->SpawnActor<TestActor>().lock();
    actor1->TestPropertyInt = 99;

    DataWriter dw1(10);
    propHandle->Serialise(actor1.get(), dw1);

    std::shared_ptr<TestActor> actor2 = engine.GetWorld().lock()->SpawnActor<TestActor>().lock();
    propHandle->Deserialise(actor2.get(), dw1);

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
