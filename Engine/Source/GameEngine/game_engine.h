#ifndef MING3D_GAMEENGINE_H
#define MING3D_GAMEENGINE_H

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
        Rendering::WindowBase* mWindow = nullptr;
        Rendering::RenderWindow* mRenderWindow = nullptr;
        SceneRenderer* mSceneRenderer = nullptr;
        Rendering::RenderTarget* mRenderTarget = nullptr;
        NetworkManager* mNetworkManager = nullptr;
        PhysicsManager* mPhysicsManager = nullptr;
        InputHandler* mInputHandler = nullptr;
        InputManager* mInputManager = nullptr;

        float mTime = 0.0f;
        float mDeltaTime = 0.0f;

        void HandleDebugStats();

	public:
		GameEngine();
		~GameEngine();

		void Initialise();
        void Start();
        void Update();

        void AddCamera(CameraComponent* inCamera);
        void RemoveCamera(CameraComponent* inCamera);

        void SetWindowSize(unsigned int width, unsigned int height);

        inline Rendering::RenderDevice* GetRenderDevice() { return mRenderDevice; }
        inline SceneRenderer* GetSceneRenderer() { return mSceneRenderer; }
        inline Rendering::WindowBase* GetMainWindow() { return mWindow; }
        inline Rendering::RenderWindow* GetMainRenderWindow() { return mRenderWindow; }
        inline World* GetWorld() { return mWorld; }
        inline Platform* GetPlatform() { return mPlatform; }
        inline NetworkManager* GetNetworkManager() { return mNetworkManager; }
        inline PhysicsManager* GetPhysicsManager() { return mPhysicsManager; }
        inline Rendering::RenderTarget* GetMainRenderTarget() { return mRenderTarget; }
        inline InputHandler* GetInputHandler() { return mInputHandler; }
        inline InputManager* GetInputManager() { return mInputManager; }
        
        float GetDeltaTime() const { return mDeltaTime; }
        float GetTime() const { return mTime; }
    };
}

namespace Ming3D
{
    extern GameEngine* GGameEngine;
}

#endif
