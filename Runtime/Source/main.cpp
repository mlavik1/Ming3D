
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

using namespace Ming3D;

int main()
{
    GameEngine* gameEngine = new GameEngine();
	gameEngine->Initialise();

    Actor* camActor = new Actor();
    camActor->AddComponent<CameraComponent>();
    camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));
    gameEngine->GetWorld()->AddActor(camActor);

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

    Actor* guiActor = new Actor();
    guiActor->GetTransform().SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    guiActor->GetTransform().SetLocalScale(glm::vec3(0.005f, 0.005f, 0.005f));
    WidgetComponent* widgetComp = guiActor->AddComponent<WidgetComponent>();
    Widget* rootWidget = new Widget();
    rootWidget->setPosition(0.0f, 0.0f, WidgetSizeMode::Absolute);
    rootWidget->setSize(800.0f, 600.0f, WidgetSizeMode::Absolute);
    ImageWidget* img1 = new ImageWidget();
    img1->setPosition(0.0f, 0.0f, WidgetSizeMode::Relative);
    img1->setSize(0.5f, 0.5f, WidgetSizeMode::Relative);
    img1->SetColour(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    rootWidget->addWidget(img1);
    ImageWidget* img2 = new ImageWidget();
    img2->setPosition(0.5f, 0.5f, WidgetSizeMode::Relative);
    img2->setSize(0.5f, 0.5f, WidgetSizeMode::Relative);
    img2->SetColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    rootWidget->addWidget(img2);
    widgetComp->SetWidget(rootWidget);
    gameEngine->GetWorld()->AddActor(guiActor);

    Texture* texxx = TextureLoader::LoadTextureData("Resources/texture.jpg");
    img2->SetTexture(texxx);
    img2->SetColour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

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
