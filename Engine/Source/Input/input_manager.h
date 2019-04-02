#ifndef MING3D_INPUTMANAGER_H
#define MING3D_INPUTMANAGER_H

#include "input_event.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace Ming3D
{
    class InputManager
    {
    public:
        void Update();
        void AddInputEvent(const InputEvent& inEvent);

        bool GetKey(KeyCode inKey);
        bool GetKeyDown(KeyCode inKey);
        bool GetKeyUp(KeyCode inKey);

    private:
        std::vector<InputEvent> mCurrentEvents;
        std::vector<InputEvent> mQueuedEvents;

        std::unordered_set<KeyCode> mKeyDownSet;
        std::unordered_set<KeyCode> mKeyUpSet;
        std::unordered_map<KeyCode, bool> mKeyPressStateMap;

        void HandleEvent(const InputEvent& inEvent);
    };
}

#endif
