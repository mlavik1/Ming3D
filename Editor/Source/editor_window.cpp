#include "editor_window.h"

#include <Windows.h>
#include "window.h"
#include "button.h"
#include "panel.h"
#include "table_layout_panel.h"
#include "scene_view.h"
#include "tree_view.h"
#include "Components/mesh_component.h" // TEMP
#include "World/world.h"
#include "GameEngine/game_engine.h"

namespace Ming3D
{
	EditorWindow::EditorWindow()
		: NativeUI::Window()
	{
		NativeUI::Window* window = new NativeUI::Window();
		window->SetTitle("Editor window");
		window->Show();
		window->Maximize();
        
        // Menu
        window->AddMenuPopup("File", "File");
        window->AddMenuPopup("Create", "Create");
        window->AddMenuPopup("Create_Actor", "Actor", "Create");
        window->AddMenuPopup("Help", "Help");

        window->AddMenuItem("File_New", "New", "File");
        window->AddMenuItem("File_Open", "Open", "File");
        window->AddMenuSeparator("File");
        window->AddMenuItem("File_Quit", "Quit", "File");
        window->AddMenuItem("Create_Actor_Actor", "Actor", "Create_Actor");

        window->SetMenuItemClickEvent("File_Quit", []() { terminate(); });
        window->SetMenuItemClickEvent("Create_Actor_Actor", [&]() 
        {
            Actor* actor1 = new Actor();
            actor1->GetTransform().SetLocalPosition(glm::vec3(-1.5f, 0.0f, 0.0f));
            actor1->GetTransform().SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));
            actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
            //MeshComponent* meshComp1 = actor1->AddComponent<MeshComponent>();
            //meshComp1->SetMesh("Resources//Mvr_PetCow_walk.dae");
            GGameEngine->GetWorld()->AddActor(actor1);
            mSceneHierarchyView->AddActor(actor1);
            mPropertyInspector->SelectActor(actor1);
        }
        );

		// Main panel
		NativeUI::TableLayoutPanel* mainPanel = new NativeUI::TableLayoutPanel(window, 1, 2);
		mainPanel->SetRowHeight(0, 0.12f);
		mainPanel->SetRowHeight(1, 0.87f);
		NativeUI::TableLayoutCell* toolbarCell = mainPanel->GetCell(0, 0);
		NativeUI::TableLayoutCell* editorMainCell = mainPanel->GetCell(0, 1);

		// Panel containing game view, content browser and inspector
		NativeUI::TableLayoutPanel* editorMainPanel = new NativeUI::TableLayoutPanel(editorMainCell->mContainerPanel, 3, 1);
		editorMainPanel->SetColumnWidth(0, 0.2f);
		editorMainPanel->SetColumnWidth(1, 0.6f);
		editorMainPanel->SetColumnWidth(2, 0.2f);
		NativeUI::TableLayoutCell* leftBrowserCell = editorMainPanel->GetCell(0, 0);
		NativeUI::TableLayoutCell* viewContainerCell = editorMainPanel->GetCell(1, 0);
		NativeUI::TableLayoutCell* propertyInspectorCell = editorMainPanel->GetCell(2, 0);

		// Panel containing game view and map editor view
		NativeUI::TableLayoutPanel* viewportsPanel = new NativeUI::TableLayoutPanel(viewContainerCell->mContainerPanel, 1, 2);
		viewportsPanel->SetRowHeight(0, 0.7f);
		viewportsPanel->SetRowHeight(1, 0.3f);
		NativeUI::TableLayoutCell* gameViewCell = viewportsPanel->GetCell(0, 0);
        NativeUI::TableLayoutCell* debugViewCell = viewportsPanel->GetCell(0, 1);

        // Panel containing scene hierarchy and content browser
		NativeUI::TableLayoutPanel* leftPanel = new NativeUI::TableLayoutPanel(leftBrowserCell->mContainerPanel, 1, 2);
        leftPanel->SetRowHeight(0, 0.5f);
        leftPanel->SetRowHeight(1, 0.5f);
		NativeUI::TableLayoutCell* sceneHierarchyCell = leftPanel->GetCell(0, 0);
        NativeUI::TableLayoutCell* contentBrowserCell = leftPanel->GetCell(0, 1);

        // Scene hierarchy
        mSceneHierarchyView = new SceneHierarchyView(sceneHierarchyCell->mContainerPanel);
        mSceneHierarchyView->SetVerticalSizeMode(NativeUI::SizeMode::Relative);
        mSceneHierarchyView->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);
        mSceneHierarchyView->SetSize(1.0f, 1.0f);

        // Property inspector
        mPropertyInspector = new PropertyInspector(propertyInspectorCell->mContainerPanel);
        mPropertyInspector->SetVerticalSizeMode(NativeUI::SizeMode::Relative);
        mPropertyInspector->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);
        mPropertyInspector->SetSize(1.0f, 1.0f);

		// Test buttons
		NativeUI::Button* testBtnToolbar = new NativeUI::Button(toolbarCell->mContainerPanel);
		testBtnToolbar->SetText("Toolbar");
		testBtnToolbar->SetSize(1.0f, 1.0f);
		testBtnToolbar->SetVerticalSizeMode(NativeUI::SizeMode::Relative);
		testBtnToolbar->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);

		// Scene view
		mSceneView = new Ming3D::SceneView(gameViewCell->mContainerPanel);
		mSceneView->SetSize(1.0f, 1.0f);
		mSceneView->SetVerticalSizeMode(NativeUI::SizeMode::Relative);
		mSceneView->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);


        for (Actor* actor : GGameEngine->GetWorld()->GetActors())
        {
            mSceneHierarchyView->AddActor(actor);
            //mPropertyInspector->SelectActor(actor);
        }
	}

    void EditorWindow::OnTick()
    {
        mSceneView->OnTick();
        mPropertyInspector->OnTick();
    }
}
