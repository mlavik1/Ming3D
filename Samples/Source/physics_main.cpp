#if MING3D_TESTTYPE == 7

#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "Debug/debug.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"
#include "Components/rigidbody_component.h"
#include "Components/box_collider_component.h"
#include "Components/camera_component.h"
#include "Model/model_helper.h"
#include "Debug/debug_graphics.h"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>
#include "SceneRenderer/render_scene_object.h" // TEMP

using namespace Ming3D;

int main()
{
    GameEngine* gameEngine = new GameEngine();
    
    gameEngine->Initialise();
    
    Actor* camActor = new Actor();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));
    camActor->GetTransform().SetWorldRotation(glm::rotate(-0.3f, glm::vec3(1.0f, 0.0f, 0.0f)));
    gameEngine->GetWorld()->AddActor(camActor);

    Actor* actor1 = new Actor();
    actor1->GetTransform().SetLocalPosition(glm::vec3(-1.0f, -2.0f, 1.0f));
    actor1->GetTransform().SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));
    actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    ModelLoader::LoadModel("Resources//Mvr_PetCow_walk.dae", actor1);
    RigidBodyComponent* rigidComp1 = actor1->AddComponent<RigidBodyComponent>();
    rigidComp1->SetKinematic(true);
    BoxColliderComponent* boxComp1 = actor1->AddComponent<BoxColliderComponent>();
    boxComp1->SetSize(glm::vec3(0.25f, 0.25f, 0.25f));
    gameEngine->GetWorld()->AddActor(actor1);
    
    Actor* actor2 = new Actor();
    actor2->GetTransform().SetLocalPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    actor2->GetTransform().SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));
    actor2->GetTransform().SetLocalRotation(glm::angleAxis(30.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    ModelLoader::LoadModel("Resources//Mvr_PetCow_walk.dae", actor2);
    RigidBodyComponent* rigidComp2 = actor2->AddComponent<RigidBodyComponent>();
    BoxColliderComponent* boxComp2 = actor2->AddComponent<BoxColliderComponent>();
    boxComp2->SetSize(glm::vec3(0.25f, 0.25f, 0.25f));
    gameEngine->GetWorld()->AddActor(actor2);

    DebugGraphics::DrawBox(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.2f, 1.2f, 1.2f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    while (true)
    {
        gameEngine->Update();
    }

    delete(gameEngine);

    return 0;
}

#endif
