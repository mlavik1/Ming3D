#ifndef MING3D_GAMEENGINE_H
#define MING3D_GAMEENGINE_H

namespace Ming3D
{
	class ClassManager;
    class World;
    class TimeManager;
    class Platform;
    class RenderDevice;
    class WindowBase;
    class RenderWindow;
    class SceneRenderer;
    class RenderTarget;
    class NetworkManager;
    class PhysicsManager;
    class CameraComponent;
    class InputHandler;
    class InputManager;

	class GameEngine
	{
	private:
		ClassManager* mClassManager = nullptr;
        Platform* mPlatform = nullptr;
        World* mWorld = nullptr;
        TimeManager* mTimeManager = nullptr;
        RenderDevice* mRenderDevice = nullptr;
        WindowBase* mWindow = nullptr;
        RenderWindow* mRenderWindow = nullptr;
        SceneRenderer* mSceneRenderer = nullptr;
        RenderTarget* mRenderTarget = nullptr;
        NetworkManager* mNetworkManager = nullptr;
        PhysicsManager* mPhysicsManager = nullptr;
        InputHandler* mInputHandler = nullptr;
        InputManager* mInputManager = nullptr;

        float mDeltaTime;

        void HandleDebugStats();

	public:
		GameEngine();
		~GameEngine();

		void Initialise();
        void Start();
        void Update();

        void AddCamera(CameraComponent* inCamera);
        void RemoveCamera(CameraComponent* inCamera);

        inline RenderDevice* GetRenderDevice() { return mRenderDevice; }
        inline SceneRenderer* GetSceneRenderer() { return mSceneRenderer; }
        inline WindowBase* GetMainWindow() { return mWindow; }
        inline RenderWindow* GetMainRenderWindow() { return mRenderWindow; }
        inline World* GetWorld() { return mWorld; }
        inline Platform* GetPlatform() { return mPlatform; }
        inline NetworkManager* GetNetworkManager() { return mNetworkManager; }
        inline PhysicsManager* GetPhysicsManager() { return mPhysicsManager; }
        inline RenderTarget* GetMainRenderTarget() { return mRenderTarget; }
        inline InputHandler* GetInputHandler() { return mInputHandler; }
        inline InputManager* GetInputManager() { return mInputManager; }
        
        float GetDeltaTime() const { return mDeltaTime; }
    };
}

namespace Ming3D
{
    extern GameEngine* GGameEngine;
}

#endif
