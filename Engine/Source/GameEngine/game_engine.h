#ifndef MING3D_GAMEENGINE_H
#define MING3D_GAMEENGINE_H

#include <vector>
#include <string>

namespace Ming3D
{
	class ClassManager;
    class World;
    class TimeManager;
    class Platform;
    class SceneRenderer;
    class NetworkManager;
    class PhysicsManager;
    class CameraComponent;
    class InputHandler;
    class InputManager;
    class LightSource;
    class RenderWindowHandle;
    class FontManager;
    class GUIResourceManager;
    namespace Rendering
    {
        class RenderDevice;
        class WindowBase;
        class RenderWindow;
        class RenderTarget;
    }

	class GameEngine
    {
	private:
		ClassManager* mClassManager = nullptr;
        Platform* mPlatform = nullptr;
        World* mWorld = nullptr;
        TimeManager* mTimeManager = nullptr;
        Rendering::RenderDevice* mRenderDevice = nullptr;
        Rendering::WindowBase* mMainWindow = nullptr;
        RenderWindowHandle* mMainRenderWindow = nullptr;
        std::vector<Rendering::WindowBase*> mWindows;
        std::vector<RenderWindowHandle*> mRenderWindows;
        SceneRenderer* mSceneRenderer = nullptr;
        NetworkManager* mNetworkManager = nullptr;
        PhysicsManager* mPhysicsManager = nullptr;
        InputHandler* mInputHandler = nullptr;
        InputManager* mInputManager = nullptr;
        FontManager* mFontManager = nullptr;
        GUIResourceManager* mGUIResourceManager = nullptr;

        float mTime = 0.0f;
        float mDeltaTime = 0.0f;

        void HandleDebugStats();

	public:
		GameEngine();
		~GameEngine();

		void Initialise();
        bool Update();

        /* Create a new RenderWindow. */
        RenderWindowHandle* CreateRenderWindow(unsigned int width, unsigned int height);

        /* Create a RenderWindow from a window. */
        RenderWindowHandle* CreateRenderWindow(Rendering::WindowBase* window);

        /* Recreate a RenderWindow. This will not recreate the actual Window, but only the render target and swap chain. */
        void RecreateRenderWindow(RenderWindowHandle* wndHandle);

        void SetMainWindowSize(unsigned int width, unsigned int height);

        std::string GetResourceDirectory();

        inline Rendering::RenderDevice* GetRenderDevice() { return mRenderDevice; }
        inline SceneRenderer* GetSceneRenderer() { return mSceneRenderer; }
        inline Rendering::WindowBase* GetMainWindow() { return mMainWindow; }
        inline RenderWindowHandle* GetMainRenderWindow() { return mMainRenderWindow; }
        inline World* GetWorld() { return mWorld; }
        inline Platform* GetPlatform() { return mPlatform; }
        inline NetworkManager* GetNetworkManager() { return mNetworkManager; }
        inline PhysicsManager* GetPhysicsManager() { return mPhysicsManager; }
        inline InputHandler* GetInputHandler() { return mInputHandler; }
        inline InputManager* GetInputManager() { return mInputManager; }
        inline FontManager* GetFontManager() { return mFontManager; }
        inline GUIResourceManager* GetGUIResourceManager() { return mGUIResourceManager; }

        float GetDeltaTime() const { return mDeltaTime; }
        float GetTime() const { return mTime; }
    };
}

namespace Ming3D
{
    extern GameEngine* GGameEngine;
}

#endif
