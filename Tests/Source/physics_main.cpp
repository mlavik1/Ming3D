#if MING3D_TESTTYPE == 7

#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "Debug/debug.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"
#include "Components/rigidbody_component.h"
#include "Components/box_collider_component.h"

#include <iostream>

using namespace Ming3D;

int main()
{
    GameEngine* gameEngine = new GameEngine();
    
    gameEngine->Initialise();
    gameEngine->Start();
    
    Actor* actor1 = new Actor();
    actor1->GetTransform().SetLocalPosition(glm::vec3(-1.0f, -2.0f, 1.0f));
    actor1->GetTransform().SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));
    actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    MeshComponent* meshComp1 = actor1->AddComponent<MeshComponent>();
    meshComp1->SetMesh("Resources//Mvr_PetCow_walk.dae");
    RigidBodyComponent* rigidComp1 = actor1->AddComponent<RigidBodyComponent>();
    rigidComp1->SetKinematic(true);
    BoxColliderComponent* boxComp1 = actor1->AddComponent<BoxColliderComponent>();
    boxComp1->SetSize(glm::vec3(0.25f, 0.25f, 0.25f));
    gameEngine->GetWorld()->AddActor(actor1);
    
    Actor* actor2 = new Actor();
    actor2->GetTransform().SetLocalPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    actor2->GetTransform().SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));
    actor2->GetTransform().SetLocalRotation(glm::angleAxis(30.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    MeshComponent* meshComp2 = actor2->AddComponent<MeshComponent>();
    meshComp2->SetMesh("Resources//Mvr_PetCow_walk.dae");
    RigidBodyComponent* rigidComp2 = actor2->AddComponent<RigidBodyComponent>();
    BoxColliderComponent* boxComp2 = actor2->AddComponent<BoxColliderComponent>();
    boxComp2->SetSize(glm::vec3(0.25f, 0.25f, 0.25f));
    gameEngine->GetWorld()->AddActor(actor2);

    while (true)
    {
        gameEngine->Update();
    }

    delete(gameEngine);

    return 0;
}

#endif
