#include "editor.h"
#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"
#include "Model/model_helper.h"
#include "Components/camera_component.h"
#include "Input/input_manager.h"
#include "Model/material_factory.h"
#include "Texture/texture.h"
#include "Texture/texture_loader.h"
#include "GUI/image_widget.h"
#include "Components/widget_component.h"
#include "GUI/text_widget.h"
#include "Debug/debug.h"
#include "GUI/widget_loader.h"
#include "SceneRenderer/camera.h"
#include "Window/render_window_handle.h"

namespace Ming3D
{
    Editor* GEditor = nullptr;

    Editor::Editor()
    {
        GEditor = this;
    }

    Editor::~Editor()
    {

    }

    void Editor::Run()
    {
        GameEngine* gameEngine = new GameEngine();
        gameEngine->Initialise();
        gameEngine->SetMainWindowSize(1600, 900);

        ActorPtr skybox = gameEngine->GetWorld().lock()->SpawnActor("Skybox");
        skybox->GetTransform().SetLocalPosition(glm::vec3(1.5f, -2.0f, -6.0f));
        skybox->GetTransform().SetLocalScale(glm::vec3(-50.0f, 50.0f, 50.0f));
        ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Skybox/Skybox.obj"), skybox.Get(), MODELLOADERFLAGS_UNLIT | MODELLOADERFLAGS_FORCE_OPAQUE);

        ActorPtr actor1 = gameEngine->GetWorld().lock()->SpawnActor("Cow");
        actor1->GetTransform().SetLocalPosition(glm::vec3(1.5f, -2.0f, -6.0f));
        actor1->GetTransform().SetLocalScale(glm::vec3(50.0f, 50.0f, 50.0f));
        actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
        ModelLoader::LoadModel(GGameEngine->GetResourceDirectory() + std::string("/Mvr_PetCow_walk.dae"), actor1.Get());

        const std::string resourceDir = std::string(MING3D_EDITOR_SOURCE_DIR) + std::string("/Resources");
        const std::string testGuiPath = resourceDir + std::string("/editor.widget");

        // Create GUI
        auto guiWorld = gameEngine->CreateWorld();

        Camera* guiCamera = new Camera();
        guiCamera->mRenderOrder = 0;
        guiCamera->mRenderTarget = GGameEngine->GetMainRenderWindow()->mRenderTarget; // TODO
        guiWorld.lock()->GetRenderScene()->AddCamera(guiCamera);

        ActorPtr guiActor = guiWorld.lock()->SpawnActor("GUI");
        guiActor->GetTransform().SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        guiActor->GetTransform().SetLocalScale(glm::vec3(0.005f, 0.005f, 0.005f));
        WidgetComponent* widgetComp = guiActor->AddComponent<WidgetComponent>();
        std::shared_ptr<Widget> rootWidget = WidgetLoader::LoadWidgetFromXML(testGuiPath);
        widgetComp->SetWidget(rootWidget);
        widgetComp->SetRenderMode(EWidgetRenderMode::Overlay);

        while (gameEngine->Update())
        {
        }
    }
}
