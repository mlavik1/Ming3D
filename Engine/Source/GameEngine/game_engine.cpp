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
#include "Window/render_window_handle.h"
#include "GUI/font_manager.h"
#include "GUI/gui_resource_manager.h"
#include <algorithm>
#include <iterator>

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
        mWorld = std::make_shared<World>(this);
        mWorlds.push_back(mWorld);
        mSceneRenderer = new SceneRenderer();
#ifdef MING3D_PHYSX
        mPhysicsManager = new PhysicsManagerPhysX();
#else
        mPhysicsManager = new PhysicsManagerNull();
#endif
        mNetworkManager = new NetworkManager();

        mFontManager = new FontManager();

        mGUIResourceManager = new GUIResourceManager();
    }

	GameEngine::~GameEngine()
	{
		delete mClassManager;
        delete mTimeManager;
        delete mMainRenderWindow;
        delete mMainWindow;
        delete mSceneRenderer;
        delete mNetworkManager;
        delete mPhysicsManager;
        delete mPlatform;
        delete mFontManager;
    }

	void GameEngine::Initialise()
	{
        mClassManager->InitialiseClasses();
        mPlatform->Initialise();
        mMainWindow = mPlatform->CreateOSWindow();
        mInputHandler = mPlatform->CreateInputHandler(mMainWindow);
        mInputHandler->Initialise(),
        mInputManager = new InputManager();
        mRenderDevice = mPlatform->CreateRenderDevice();
        mMainRenderWindow = CreateRenderWindow(mMainWindow);
        mTimeManager->Initialise();
        mSceneRenderer->Initialise();

        mPhysicsManager->CreatePhysicsScene();

        LOG_INFO() << "Engine resource directory: " << GetResourceDirectory();
	}

    bool GameEngine::Update()
    {
        if (!mMainWindow->IsOpen())
            return false;
        
        mTimeManager->UpdateTime();
        float deltaTime = mTimeManager->GetDeltaTimeSeconds();
        mDeltaTime = deltaTime;
        mTime += mDeltaTime;

        mPlatform->Update();
        mInputHandler->Update();
        mInputManager->Update();

        // If main window was closed this frame
        if (!mMainWindow->IsOpen())
            return false;

        mPhysicsManager->SimulateScenes(deltaTime);

        for (auto world : mWorlds)
        {
            world->Tick(deltaTime);
        }

        mNetworkManager->UpdateNetworks();

        std::vector<RenderScene*> renderScenes;
        std::transform(mWorlds.cbegin(), mWorlds.cend(), std::back_inserter(renderScenes), [](auto world) { return world->GetRenderScene(); });
        mSceneRenderer->Render(renderScenes);

        HandleDebugStats();

        return true;
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

    RenderWindowHandle* GameEngine::CreateRenderWindow(unsigned int width, unsigned int height)
    {
        Rendering::WindowBase* window = mPlatform->CreateOSWindow();
        mWindows.push_back(window);
        return CreateRenderWindow(window);
    }

    RenderWindowHandle* GameEngine::CreateRenderWindow(Rendering::WindowBase* window)
    {
        RenderWindowHandle* windowHandle = new RenderWindowHandle();
        windowHandle->mRenderWindow = mPlatform->CreateRenderWindow(window, mRenderDevice);
        windowHandle->mRenderTarget = mRenderDevice->CreateRenderTarget(windowHandle->mRenderWindow);
        mRenderWindows.push_back(windowHandle);
        return windowHandle;
    }

    void GameEngine::RecreateRenderWindow(RenderWindowHandle* wndHandle)
    {
        Rendering::WindowBase* window = wndHandle->mRenderWindow->GetWindow();
        delete wndHandle->mRenderWindow;
        wndHandle->mRenderWindow = mRenderDevice->CreateRenderWindow(window);
        wndHandle->mRenderTarget = mRenderDevice->CreateRenderTarget(wndHandle->mRenderWindow);
    }

    void GameEngine::SetMainWindowSize(unsigned int width, unsigned int height)
    {
        Rendering::WindowBase* window = mMainRenderWindow->mRenderWindow->GetWindow();
        window->SetSize(width, height);
        RecreateRenderWindow(mMainRenderWindow);
    }

    std::string GameEngine::GetResourceDirectory()
    {
        return std::string(MING3D_ENGINE_SOURCE_DIR) + std::string("/Resources");
    }

    std::weak_ptr<World> GameEngine::CreateWorld()
    {
        std::shared_ptr<World> world = std::make_shared<World>(this);
        mWorlds.push_back(world);
        return world;
    }

    void GameEngine::DestroyWorld(World* world)
    {
        for (std::list<std::shared_ptr<World>>::iterator iter = mWorlds.begin(); iter != mWorlds.end(); ++iter)
        {
            if (iter->get() == world)
            {
                mWorlds.erase(iter);
                break;
            }
        }
    }
}
