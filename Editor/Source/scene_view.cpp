#include "scene_view.h"

#include "GameEngine/game_engine.h"
#include "Platform/platform.h"
#include "winapi_window.h"
#include "GameEngine/game_engine.h"
#include "SceneRenderer/scene_renderer.h"
#include "sdl_window.h"

#include "render_device_gl.h"
#include "Platform/platform_win32.h"
#include "render_device_d3d11.h"
#include "render_window_d3d11.h"
#include "winapi_window.h"
#include "SceneRenderer/camera.h"
#include "SceneRenderer/scene_renderer.h"
#include "Actors/actor.h"
#include "Components/mesh_component.h"
#include "World/world.h"
#include "Model/model_helper.h"

namespace Ming3D
{
	SceneView::SceneView(Control* arg_parent)
		: UserControl(arg_parent)
	{

        Platform* platform = new PlatformWin32();
        platform->Initialise();

        mMainWindow = new WinAPIWindow(mHwnd);
        mMainWindow->Initialise();

        mRenderDevice = GGameEngine->GetRenderDevice();
        mRenderWindow = mRenderDevice->CreateRenderWindow(mMainWindow);
        mRenderTarget = mRenderDevice->CreateRenderTarget(mRenderWindow);

        Camera* camera = new Camera();
        camera->mRenderTarget = mRenderTarget;
        GGameEngine->GetSceneRenderer()->AddCamera(camera);

        Actor* actor1 = new Actor();
        actor1->GetTransform().SetLocalPosition(glm::vec3(1.5f, 0.0f, 0.0f));
        actor1->GetTransform().SetLocalScale(glm::vec3(2.0f, 2.0f, 2.0f));
        actor1->GetTransform().SetLocalRotation(glm::angleAxis(10.0f * 3.141592654f / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
        ModelLoader::LoadModel("Resources//Mvr_PetCow_walk.dae", actor1);
        GGameEngine->GetWorld()->AddActor(actor1);
	}

	void SceneView::OnPaint()
	{
	}

    void SceneView::OnTick()
    {
        mRenderDevice->BeginRenderWindow(mRenderWindow);
        mRenderDevice->EndRenderWindow(mRenderWindow);
    }

	bool SceneView::HasFocus()
	{
		return GetActiveWindow() == mHwnd;
	}
}