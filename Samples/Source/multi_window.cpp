#if MING3D_TESTTYPE == 9

#include "GameEngine/game_engine.h"
#include "Object/object_ptr.h"
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
#include <memory>

using namespace Ming3D;

int main()
{
    GameEngine* gameEngine = new GameEngine();
    gameEngine->Initialise();
    gameEngine->SetMainWindowSize(1366, 768);

    ActorPtr camActor = gameEngine->GetWorld().lock()->SpawnActor();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));

    RenderWindowHandle* window2 = gameEngine->CreateRenderWindow(800, 600);
    ActorPtr camActor2 = gameEngine->GetWorld().lock()->SpawnActor();
    CameraComponent* camComp = camActor2->AddComponent<CameraComponent>();
    camComp->SetRenderTarget(window2);
    camActor2->GetTransform().SetWorldPosition(glm::vec3(6.0f, 2.0f, 0.0f));
    camActor2->GetTransform().Rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    ActorPtr lightActor = gameEngine->GetWorld().lock()->SpawnActor();
	LightComponent* lightComp = lightActor->AddComponent<LightComponent>();
	lightComp->SetShadowType(EShadowType::HardShadows);
    lightActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 10.0f, 6.0f));
    lightActor->GetTransform().SetWorldRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    ActorPtr skybox = gameEngine->GetWorld().lock()->SpawnActor();
    skybox->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    skybox->GetTransform().SetLocalScale(glm::vec3(-50.0f, 50.0f, 50.0f));
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Skybox/Skybox.obj"), skybox.get(), MODELLOADERFLAGS_UNLIT);

    ActorPtr actor1 = gameEngine->GetWorld().lock()->SpawnActor();
    actor1->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    actor1->GetTransform().SetLocalScale(glm::vec3(20.0f, 20.0f, 20.0f));
    actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Mvr_PetCow_walk.dae"), actor1.get());
    for (MeshComponent* currMeshComp : actor1->GetComponentsInChildren<MeshComponent>())
    {
        currMeshComp->GetMaterial()->SetCastShadows(true);
    }

    ActorPtr actor2 = gameEngine->GetWorld().lock()->SpawnActor();
    actor2->GetTransform().SetLocalPosition(glm::vec3(-1.5f, 0.0f, 0.0f));
    actor2->GetTransform().SetLocalScale(glm::vec3(20.0f, 20.0f, 20.0f));
    actor2->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Mvr_PetCow_walk.dae"), actor2.get());
    for (MeshComponent* currMeshComp : actor2->GetComponentsInChildren<MeshComponent>())
    {
        currMeshComp->GetMaterial()->SetCastShadows(true);
    }

    ActorPtr planeObj = gameEngine->GetWorld().lock()->SpawnActor();
    planeObj->GetTransform().SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    std::shared_ptr<Mesh> planeMesh = PrimitiveFactory::CreatePlane(glm::vec2(100.0f, 100.0f), 2, 2);
    MeshComponent* planeMeshComp = planeObj->AddComponent<MeshComponent>();
    planeMeshComp->SetMesh(planeMesh);
    std::shared_ptr<Material> planeMat = MaterialFactory::CreateMaterial(GGameEngine->GetResourceDirectory() + std::string("/Shaders/defaultshader.cgp"));
    planeMat->SetTexture(0, std::shared_ptr<Texture>(TextureLoader::LoadTextureData(GGameEngine->GetResourceDirectory() + std::string("/grass.png")))); // TODO: create override with only one parameter
    planeMat->SetShaderUniformVec2("_textureTiling", glm::vec2(10.0f, 10.0f));
	planeMat->SetReceiveShadows(true);
    planeMeshComp->SetMaterial(planeMat);

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
