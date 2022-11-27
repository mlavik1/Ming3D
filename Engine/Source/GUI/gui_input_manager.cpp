#include "gui_input_manager.h"
#include "Input/input_manager.h"

namespace Ming3D
{
    GUIInputManager::GUIInputManager(InputManager* inputManager)
    {
        mInputManager = inputManager;
    }
}
