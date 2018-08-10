#include <iostream>

#include "CoreTest.h"
#include "Object/object.h"

#include "Object/function.h"
#include "GameEngine/game_engine.h"

#include "World/world.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"

using namespace Ming3D;


int main()
{
	GameEngine* gameEngine = new GameEngine();
	gameEngine->Initialise();

    Actor* actor1 = new Actor();
    MeshComponent* meshComp1 = actor1->AddComponent<MeshComponent>();
    meshComp1->SetMesh("Resources//Mvr_PetCow_walk.dae");
    gameEngine->GetWorld()->AddActor(actor1);;

    gameEngine->Start();

	// TESTS:

	std::vector<int> intVec({ 1, 3, 7 });
	int* intPtr = new int(3);

	Function* funcTestFunction = CoreTestClass::GetStaticClass()->GetFunctionByName("IntBoolTestFunction");
	Function* funcStringTestFunction = CoreTestClass::GetStaticClass()->GetFunctionByName("StringTestFunction");
	Function* funcIntVectorTestFunction = CoreTestClass::GetStaticClass()->GetFunctionByName("IntVectorTestFunction");
	Function* funcIntPointerTestFunction = CoreTestClass::GetStaticClass()->GetFunctionByName("IntPointerTestFunction");
	Function* funcObjectTestFunction = CoreTestClass::GetStaticClass()->GetFunctionByName("ObjectTestFunction");

	CoreTestClass* testMingObject = new CoreTestClass();

	testMingObject->CallFunction(funcTestFunction, FunctionArgs({ FunctionParam<int>(3), FunctionParam<bool>(true)}));
	testMingObject->CallFunction(funcStringTestFunction, FunctionArgs({ FunctionParam<std::string>("test")}));
	testMingObject->CallFunction(funcIntVectorTestFunction, FunctionArgs({ FunctionParam<std::vector<int>>(intVec) }));
	testMingObject->CallFunction(funcIntPointerTestFunction, FunctionArgs({ FunctionParam<int*>(intPtr) }));

	FunctionArgs intVecArgs1({ FunctionParam<std::vector<int>>(intVec) });
	DataWriter dataWriter(1);
	funcIntVectorTestFunction->SerialiseFunctionArgs(intVecArgs1, dataWriter);
	FunctionArgs intVecArgs2 = funcIntVectorTestFunction->DeserialiseFunctionArgs(dataWriter);

	testMingObject->CallFunction(funcIntVectorTestFunction, intVecArgs2);

	TestClass testClass;
	testClass.mTestValue = 3;
	testMingObject->CallFunction(funcObjectTestFunction, FunctionArgs({ FunctionParam<TestClass>(testClass)}));


	while (true)
	{

	}

	return 0;
}