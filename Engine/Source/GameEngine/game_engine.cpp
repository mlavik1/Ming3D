#include "game_engine.h"

#include "ClassManager/class_manager.h"
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
#include "Components/camera_component.h"
#include "Input/input_handler.h"
#include "Input/input_manager.h"
#include "Debug/debug_stats.h"

#ifdef MING3D_PHYSX
#include "Physics/API/PhysX/physics_manager_physx.h"
#else
#include "Physics/API/Null/physics_manager_null.h"
#endif

#ifdef _WIN32
#include "Source/Platform/platform_win32.h"
#else
#include "Platform/platform_linux.h"
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
        mPlatform = new PlatformLinux();
#endif
        mTimeManager = new TimeManager();
        mWorld = new World();
        mSceneRenderer = new SceneRenderer();
#ifdef MING3D_PHYSX
        mPhysicsManager = new PhysicsManagerPhysX();
#else
        mPhysicsManager = new PhysicsManagerNull();
#endif
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
        mSceneRenderer->Initialise();

        mPhysicsManager->CreatePhysicsScene();
	}

    void GameEngine::Update()
    {
        mTimeManager->UpdateTime();
        float deltaTime = mTimeManager->GetDeltaTimeSeconds();
        mDeltaTime = deltaTime;
        mTime += mDeltaTime;

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

    void GameEngine::SetWindowSize(unsigned int width, unsigned int height)
    {
        if(mRenderTarget != nullptr)
            delete mRenderTarget;
        mRenderWindow->GetWindow()->SetSize(width, height);
        mRenderTarget = mRenderDevice->CreateRenderTarget(mRenderWindow);
    }

    void GameEngine::Start()
    {

    }
}
