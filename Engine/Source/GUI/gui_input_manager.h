#pragma once

namespace Ming3D
{
    class InputManager;

    class GUIInputManager
    {
    private:
        InputManager* mInputManager;

    public:
        GUIInputManager(InputManager* inputManager);
    };
}
