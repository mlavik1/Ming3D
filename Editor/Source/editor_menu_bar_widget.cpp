#include "editor_menu_bar_widget.h"
#include "GameEngine/game_engine.h"
#include "Platform/platform.h"
#include "Platform/platform_interface.h"
#include "Model/model_helper.h"
#include "World/world.h"

IMPLEMENT_CLASS(Ming3D::EditorMenuBarWidget)

namespace Ming3D
{
    EditorMenuBarWidget::EditorMenuBarWidget()
    {
        MenuBarItem fileItem;
        fileItem.mText = "File";
        MenuBarItem newSceneItem;
        newSceneItem.mText = "New scene";
        MenuBarItem improtModelItem;
        improtModelItem.mText = "Import model";
        improtModelItem.mOnClick = [&]() { CallbackImportModel(); };
        MenuBarItem quitItem;
        quitItem.mText = "Quit";
        fileItem.mChildren = { newSceneItem, improtModelItem, quitItem };

        MenuBarItem assetsItem;
        assetsItem.mText = "Assets";
        SetItems({fileItem, assetsItem});
    }

    void EditorMenuBarWidget::CallbackImportModel()
    {
        std::string filePath;
        if (GGameEngine->GetPlatform()->GetPlatformInterface()->OpenFileDialog("Select model to load", filePath))
        {
            ActorPtr actor = GGameEngine->GetWorld().lock()->SpawnActor();
            ModelLoader::LoadModel(filePath.c_str(), actor.Get());
        }
    }
}
