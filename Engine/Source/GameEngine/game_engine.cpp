#include "game_engine.h"

#include "ClassManager/class_manager.h"
#include "basic_sample.h"
#include "rendertotexture_sample.h"
#include "World/world.h"
#include "Actors/actor.h"
#include "Components/component.h"
#include "Time/time_manager.h"
#include "Debug/debug.h"
#include "render_device.h"
#include "window_base.h"
#include "render_window.h"
#include "SceneRenderer/scene_renderer.h"
#include "Networking/network_manager.h"

#ifdef _WIN32
#include "Source/Platform/platform_win32.h"
#else
// TODO
#endif

namespace Ming3D
{
    GameEngine* GGameEngine = nullptr;

	GameEngine::GameEngine()
	{
        GGameEngine = this;

		mClassManager = new ClassManager();
#ifdef _WIN32
        mPlatform = new PlatformWin32();
#else
        // TODO
#endif
        mTimeManager = new TimeManager();
        mWorld = new World();
        mSceneRenderer = new SceneRenderer();
        mNetworkManager = new NetworkManager();
    }

	GameEngine::~GameEngine()
	{
		delete mClassManager;
        delete mWorld;
        delete mTimeManager;
        delete mRenderWindow;
        delete mWindow;
        delete mSceneRenderer;
        delete mNetworkManager;
        delete mPlatform;
    }

	void GameEngine::Initialise()
	{
        mClassManager->InitialiseClasses();
        mPlatform->Initialise();
        mWindow = mPlatform->CreateOSWindow();
        mRenderDevice = mPlatform->CreateRenderDevice();
        mRenderWindow = mPlatform->CreateRenderWindow(mWindow, mRenderDevice);
        mRenderTarget = mRenderDevice->CreateRenderTarget(mRenderWindow);
        mTimeManager->Initialise();

        Camera* camera = new Camera();
        camera->mRenderTarget = mRenderTarget;
        mSceneRenderer->AddCamera(camera);
	}

    void GameEngine::Update()
    {
        mTimeManager->UpdateTime();
        float deltaTime = mTimeManager->GetDeltaTimeSeconds();

        for (Actor* actor : mWorld->GetActors())
        {
            actor->Tick(deltaTime);
            for (Component* comp : actor->GetComponents())
            {
                comp->Tick(deltaTime);
            }
        }

        mNetworkManager->UpdateNetworks();

        mRenderDevice->BeginRenderWindow(mRenderWindow);
        mSceneRenderer->RenderCameras();
        mRenderDevice->EndRenderWindow(mRenderWindow);
}

    void GameEngine::Start()
    {
        
    }
}
