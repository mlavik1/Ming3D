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
#include "Physics/physics_manager.h"
#include "Components/camera_component.h"
#include "Input/input_handler.h"
#include "Input/input_manager.h"
#include "Debug/debug_stats.h"

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
        mPhysicsManager = new PhysicsManager();
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
        delete mPhysicsManager;
        delete mPlatform;
    }

	void GameEngine::Initialise()
	{
        mClassManager->InitialiseClasses();
        mPlatform->Initialise();
        mWindow = mPlatform->CreateOSWindow();
        mInputHandler = mPlatform->CreateInputHandler();
        mInputManager = new InputManager();
        mRenderDevice = mPlatform->CreateRenderDevice();
        mRenderWindow = mPlatform->CreateRenderWindow(mWindow, mRenderDevice);
        mRenderTarget = mRenderDevice->CreateRenderTarget(mRenderWindow);
        mTimeManager->Initialise();

        mPhysicsManager->CreatePhysicsScene();
	}

    void GameEngine::Update()
    {
        mTimeManager->UpdateTime();
        float deltaTime = mTimeManager->GetDeltaTimeSeconds();
        mDeltaTime = deltaTime;

        mPlatform->Update();
        mInputHandler->Update();
        mInputManager->Update();

        mPhysicsManager->SimulateScenes(deltaTime);

        for (Actor* actor : mWorld->GetActors())
        {
            actor->Tick(deltaTime);
        }

        mNetworkManager->UpdateNetworks();

        mRenderDevice->BeginRenderWindow(mRenderWindow);
        mSceneRenderer->Render();
        mRenderDevice->EndRenderWindow(mRenderWindow);

        HandleDebugStats();
    }

    void GameEngine::HandleDebugStats()
    {
#ifdef MING3D_DEBUG_STATS_ENABLED
        // TODO: use other key combination
        if (mInputManager->GetKeyDown(KeyCode::Key_Space))
        {
            DebugStats_PrintAllStats();
        }
#endif
        ClearFrameStats();
    }

    void GameEngine::AddCamera(CameraComponent* inCamera)
    {
        mSceneRenderer->AddCamera(inCamera->GetCamera());
    }

    void GameEngine::RemoveCamera(CameraComponent* inCamera)
    {
        mSceneRenderer->RemoveCamera(inCamera->GetCamera());
    }

    void GameEngine::Start()
    {
        
    }
}
