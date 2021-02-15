#if MING3D_TESTTYPE == 9

#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"
#include "Model/model_helper.h"
#include "Components/camera_component.h"
#include "Components/light_component.h"
#include "glm/gtx//rotate_vector.hpp"
#include "Input/input_manager.h"
#include "Model/primitive_factory.h"
#include "Model/material_factory.h"
#include "Texture/texture.h"
#include "Texture/texture_loader.h"

using namespace Ming3D;

int main()
{
    GameEngine* gameEngine = new GameEngine();
    gameEngine->Initialise();
    gameEngine->SetMainWindowSize(1366, 768);

    Actor* camActor = new Actor();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));
    gameEngine->GetWorld()->AddActor(camActor);

    RenderWindowHandle* window2 = gameEngine->CreateRenderWindow(800, 600);
    Actor* camActor2 = new Actor();
    CameraComponent* camComp = camActor2->AddComponent<CameraComponent>();
    camComp->SetRenderTarget(window2);
    camActor2->GetTransform().SetWorldPosition(glm::vec3(6.0f, 2.0f, 0.0f));
    camActor2->GetTransform().Rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    gameEngine->GetWorld()->AddActor(camActor2);
    
    Actor* lightActor = new Actor();
	LightComponent* lightComp = lightActor->AddComponent<LightComponent>();
	lightComp->SetShadowType(EShadowType::HardShadows);
    lightActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 10.0f, 6.0f));
    lightActor->GetTransform().SetWorldRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    gameEngine->GetWorld()->AddActor(lightActor);

    Actor* skybox = new Actor();
    skybox->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    skybox->GetTransform().SetLocalScale(glm::vec3(-50.0f, 50.0f, 50.0f));
    gameEngine->GetWorld()->AddActor(skybox);
    ModelLoader::LoadModel("Resources//Skybox//Skybox.obj", skybox, MODELLOADERFLAGS_UNLIT);

    Actor* actor1 = new Actor();
    actor1->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    actor1->GetTransform().SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));
    actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    gameEngine->GetWorld()->AddActor(actor1);
    ModelLoader::LoadModel("Resources//Mvr_PetCow_walk.dae", actor1);
    for (MeshComponent* currMeshComp : actor1->GetComponentsInChildren<MeshComponent>())
    {
        currMeshComp->GetMaterial()->SetCastShadows(true);
    }

    Actor* actor2 = new Actor();
    actor2->GetTransform().SetLocalPosition(glm::vec3(-1.5f, 0.0f, 0.0f));
    actor2->GetTransform().SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));
    actor2->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    gameEngine->GetWorld()->AddActor(actor2);
    ModelLoader::LoadModel("Resources//Mvr_PetCow_walk.dae", actor2);
    for (MeshComponent* currMeshComp : actor2->GetComponentsInChildren<MeshComponent>())
    {
        currMeshComp->GetMaterial()->SetCastShadows(true);
    }

    Actor* planeObj = new Actor();
    planeObj->GetTransform().SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    Mesh* planeMesh = PrimitiveFactory::CreatePlane(glm::vec2(100.0f, 100.0f), 2, 2);
    MeshComponent* planeMeshComp = planeObj->AddComponent<MeshComponent>();
    planeMeshComp->SetMesh(planeMesh);
    Material* planeMat = MaterialFactory::CreateMaterial("Resources/Shaders/defaultshader.cgp");
    planeMat->SetTexture(0, TextureLoader::LoadTextureData("Resources/grass.png")); // TODO: create override with only one parameter
    planeMat->SetShaderUniformVec2("_textureTiling", glm::vec2(10.0f, 10.0f));
	planeMat->SetReceiveShadows(true);
    planeMeshComp->SetMaterial(planeMat);
    gameEngine->GetWorld()->AddActor(planeObj);

    const float camSpeed = 3.0f;
    const float camRotSpeed = 1.0f;

    while (true)
    {
        gameEngine->Update();
        actor1->GetTransform().Rotate(0.001f, glm::vec3(0,1,0));

        float speed = camSpeed * GGameEngine->GetDeltaTime();
        float rotSpeed = camRotSpeed * GGameEngine->GetDeltaTime();
        Transform& camTrans = camActor->GetTransform();

        glm::vec3 camPos = camTrans.GetWorldPosition();
        if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_W))
            camPos += camTrans.GetForward() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_S))
            camPos += camTrans.GetForward() * -speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_D))
            camPos += camTrans.GetRight() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_A))
            camPos += camTrans.GetRight() * -speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_E))
            camPos += camTrans.GetUp() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Q))
            camPos -= camTrans.GetUp() * speed;

        camTrans.SetWorldPosition(camPos);

        if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Right))
            camTrans.Rotate(-rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Left))
            camTrans.Rotate(rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Up))
            camTrans.Rotate(-rotSpeed, glm::vec3(1.0f, 0.0f, 0.0f));
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Down))
            camTrans.Rotate(rotSpeed, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    return 0;
}


#endif
