#ifndef MING3D_INPUTMANAGER_H
#define MING3D_INPUTMANAGER_H

#include "input_event.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "glm/vec2.hpp"

namespace Ming3D
{
    class InputManager
    {
    private:
        std::vector<InputEvent> mCurrentEvents;
        std::vector<InputEvent> mQueuedEvents;

        std::unordered_set<KeyCode> mKeyDownSet;
        std::unordered_set<KeyCode> mKeyUpSet;
        std::unordered_map<KeyCode, bool> mKeyPressStateMap;
        std::unordered_map<EAxis2D, glm::vec2> mAxisValues;
        glm::ivec2 mMousePosition;

        void HandleEvent(const InputEvent& inEvent);

    public:
        void Update();
        void AddInputEvent(const InputEvent& inEvent);

        bool GetKey(KeyCode inKey);
        bool GetKeyDown(KeyCode inKey);
        bool GetKeyUp(KeyCode inKey);
        glm::ivec2 GetMousePosition() const;
        glm::vec2 GetAxis2D(EAxis2D axis) const;

        const std::vector<InputEvent>& GetEvents() const { return mCurrentEvents; }
    };
}

#endif
