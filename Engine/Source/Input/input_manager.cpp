#include "input_manager.h"

namespace Ming3D
{
    void InputManager::Update()
    {
        mKeyDownSet.clear();
        mKeyUpSet.clear();

        for (const InputEvent& evt : mQueuedEvents)
        {
            HandleEvent(evt);
        }

        mCurrentEvents = mQueuedEvents;
        mQueuedEvents.clear();
    }

    void InputManager::AddInputEvent(const InputEvent& inEvent)
    {
        mQueuedEvents.push_back(inEvent);
    }

    bool InputManager::GetKey(KeyCode inKey)
    {
        auto iter = mKeyPressStateMap.find(inKey);
        return iter != mKeyPressStateMap.end() && iter->second;
    }

    bool InputManager::GetKeyDown(KeyCode inKey)
    {
        auto iter = mKeyDownSet.find(inKey);
        return iter != mKeyDownSet.end();
    }

    bool InputManager::GetKeyUp(KeyCode inKey)
    {
        auto iter = mKeyUpSet.find(inKey);
        return iter != mKeyDownSet.end();
    }

    void InputManager::HandleEvent(const InputEvent& inEvent)
    {
        switch (inEvent.mType)
        {
        case InputEventType::KeyDown:
        {
            mKeyDownSet.insert(inEvent.mKey.mKeyCode);
            mKeyPressStateMap[inEvent.mKey.mKeyCode] = true;
            break;
        }
        case InputEventType::KeyUp:
        {
            mKeyUpSet.insert(inEvent.mKey.mKeyCode);
            mKeyPressStateMap[inEvent.mKey.mKeyCode] = false;
            break;
        }
        }
    }
}
