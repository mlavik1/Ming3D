#include "editor_window.h"

#include <Windows.h>
#include "window.h"
#include "button.h"
#include "panel.h"
#include "table_layout_panel.h"
#include "scene_view.h"

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
		NativeUI::TableLayoutCell* contentBrowserCell = editorMainPanel->GetCell(0, 0);
		NativeUI::TableLayoutCell* viewContainerCell = editorMainPanel->GetCell(1, 0);
		NativeUI::TableLayoutCell* propertyInspectorCell = editorMainPanel->GetCell(2, 0);

		// Panel containing game view and map editor view
		NativeUI::TableLayoutPanel* viewportsPanel = new NativeUI::TableLayoutPanel(viewContainerCell->mContainerPanel, 1, 2);
		viewportsPanel->SetRowHeight(0, 0.7f);
		viewportsPanel->SetRowHeight(1, 0.3f);
		NativeUI::TableLayoutCell* gameViewCell = viewportsPanel->GetCell(0, 0);
        NativeUI::TableLayoutCell* debugViewCell = viewportsPanel->GetCell(0, 1);

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

		//mapEditorViewCell->mContainerPanel->ShowScrollbar(true);
	}

    void EditorWindow::OnTick()
    {
        mSceneView->OnTick();
    }
}
