#if MING3D_TESTTYPE == 10
#include <iostream>

#include "CoreTest.h"
#include "Object/object.h"

#include "Object/function.h"
#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"
#include "Model/model_helper.h"
#include "Components/camera_component.h"
#include "glm/gtx//rotate_vector.hpp"
#include "Input/input_manager.h"
#include "Model/primitive_factory.h"
#include "Model/material_factory.h"
#include "Texture/texture.h"
#include "Texture/texture_loader.h"
#include "GUI/image_widget.h"
#include "Components/widget_component.h"
#include "GUI/text_widget.h"
#include <vector>
#include <string>
#include <memory>
#include "Debug/debug.h"
#include "GUI/widget_loader.h"

using namespace Ming3D;

int main()
{
    GameEngine* gameEngine = new GameEngine();
    gameEngine->Initialise();
    gameEngine->SetMainWindowSize(1280, 720);

    auto world = gameEngine->GetWorld().lock();

    Actor* camActor = world->SpawnActor();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));

    Actor* skybox = world->SpawnActor();
    skybox->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    skybox->GetTransform().SetLocalScale(glm::vec3(-50.0f, 50.0f, 50.0f));
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Skybox/Skybox.obj"), skybox, MODELLOADERFLAGS_UNLIT | MODELLOADERFLAGS_FORCE_OPAQUE);

    Actor* actor1 = world->SpawnActor();
    actor1->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
    actor1->GetTransform().SetLocalScale(glm::vec3(50.0f, 50.0f, 50.0f));
    actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
    ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Mvr_PetCow_walk.dae"), actor1);


    const std::string resourceDir = std::string(MING3D_SAMPLES_SOURCE_DIR) + std::string("/Resources");
    const std::string testGuiPath = resourceDir + std::string("/menu_test.widget");
    //const std::string testGuiPath = resourceDir + std::string("/gui_test.widget");

    Actor* guiActor = world->SpawnActor();
    guiActor->GetTransform().SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    guiActor->GetTransform().SetLocalScale(glm::vec3(0.005f, 0.005f, 0.005f));
    WidgetComponent* widgetComp = guiActor->AddComponent<WidgetComponent>();
    std::shared_ptr<Widget> rootWidget = WidgetLoader::LoadWidgetFromXML(testGuiPath);
    widgetComp->SetWidget(rootWidget);
    widgetComp->SetRenderMode(EWidgetRenderMode::Overlay);

    const float camSpeed = 3.0f;
    const float camRotSpeed = 1.0f;

    while (gameEngine->Update())
    {
        actor1->GetTransform().Rotate(0.001f, glm::vec3(0, 1, 0));

        float speed = camSpeed * GGameEngine->GetDeltaTime();
        float rotSpeed = camRotSpeed * GGameEngine->GetDeltaTime();
        Transform& camTrans = camActor->GetTransform();

        glm::vec3 camPos = camTrans.GetWorldPosition();
        if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_W))
            camPos += camTrans.GetUp() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_S))
            camPos += camTrans.GetUp() * -speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_D))
            camPos += camTrans.GetRight() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_A))
            camPos += camTrans.GetRight() * -speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Up))
            camPos += camTrans.GetForward() * speed;
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Down))
            camPos += camTrans.GetForward() * -speed;

        camTrans.SetWorldPosition(camPos);

        if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Right))
            camTrans.Rotate(-rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
        else if (GGameEngine->GetInputManager()->GetKey(KeyCode::Key_Left))
            camTrans.Rotate(rotSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    return 0;
}

#endif
