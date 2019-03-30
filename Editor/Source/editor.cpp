#include "editor.h"

#include <iostream>
#include <Windows.h>
#include <functional>
#include "GameEngine/game_engine.h"
#include "World/world.h"
#include "Components/camera_component.h"

namespace Ming3D
{
	Editor* GEditor = nullptr;

	Editor::Editor()
	{

	}

	Editor* Editor::CreateEditor()
	{
		if (GEditor != nullptr)
		{
			delete GEditor;
		}
		GEditor = new Editor();
		return GEditor;
	}

	void Editor::RunEditor()
	{
		
        GameEngine* engine = new GameEngine();
        engine->Initialise();

        // TODO: Create/load scene

        Actor* camActor = new Actor();
        camActor->AddComponent<CameraComponent>();
        camActor->GetTransform().SetWorldPosition(glm::vec3(0.0f, 2.0f, 6.0f));
        engine->GetWorld()->AddActor(camActor);

		mEditorWindow = new EditorWindow();

        bool running = true;
		while(running)
		{
			MSG msg;
			msg.message = ~WM_QUIT;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

                engine->Update();
                mEditorWindow->OnTick();
			}
			if (msg.message == WM_QUIT)
			{
				//engine->Shutdown();
			}
		};

		
	}

}
