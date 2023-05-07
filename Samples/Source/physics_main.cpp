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

using namespace Ming3D;

int main()
{
    GameEngine* gameEngine = new GameEngine();
    
    gameEngine->Initialise();
    
    std::weak_ptr<Actor> camActor = gameEngine->GetWorld().lock()->SpawnActor();
    camActor.lock()->AddComponent<CameraComponent>();
    camActor.lock()->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));
    camActor.lock()->GetTransform().SetWorldRotation(glm::rotate(-0.3f, glm::vec3(1.0f, 0.0f, 0.0f)));

    std::weak_ptr<Actor> actor1 = gameEngine->GetWorld().lock()->SpawnActor();
    actor1.lock()->GetTransform().SetLocalPosition(glm::vec3(-1.0f, -2.0f, 1.0f));
    actor1.lock()->GetTransform().SetLocalScale(glm::vec3(50.0f, 50.0f, 50.0f));
    actor1.lock()->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Mvr_PetCow_walk.dae"), actor1.lock().get());
    RigidBodyComponent* rigidComp1 = actor1.lock()->AddComponent<RigidBodyComponent>();
    rigidComp1->SetKinematic(true);
    BoxColliderComponent* boxComp1 = actor1.lock()->AddComponent<BoxColliderComponent>();
    boxComp1->SetSize(glm::vec3(0.01f, 0.01f, 0.01f));
    
    std::weak_ptr<Actor> actor2 = gameEngine->GetWorld().lock()->SpawnActor();
    actor2.lock()->GetTransform().SetLocalPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    actor2.lock()->GetTransform().SetLocalScale(glm::vec3(50.0f, 50.0f, 50.0f));
    actor2.lock()->GetTransform().SetLocalRotation(glm::angleAxis(30.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f)));
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Mvr_PetCow_walk.dae"), actor2.lock().get());
    RigidBodyComponent* rigidComp2 = actor2.lock()->AddComponent<RigidBodyComponent>();
    BoxColliderComponent* boxComp2 = actor2.lock()->AddComponent<BoxColliderComponent>();
    boxComp2->SetSize(glm::vec3(0.01f, 0.01f, 0.01f));

    DebugGraphics::DrawBox(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.2f, 1.2f, 1.2f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    while (true)
    {
        gameEngine->Update();
    }

    delete(gameEngine);

    return 0;
}

#endif
