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

    bool InputManager::GetKey(KeyCode inKey) const
    {
        auto iter = mKeyPressStateMap.find(inKey);
        return iter != mKeyPressStateMap.end() && iter->second;
    }

    bool InputManager::GetKeyDown(KeyCode inKey) const
    {
        auto iter = mKeyDownSet.find(inKey);
        return iter != mKeyDownSet.end();
    }

    bool InputManager::GetKeyUp(KeyCode inKey) const
    {
        auto iter = mKeyUpSet.find(inKey);
        return iter != mKeyUpSet.end();
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
        case InputEventType::Axis2D:
        {
            mAxisValues[inEvent.mAxis.mAxis] = inEvent.mAxis.mValue;
            break;
        }
        case InputEventType::MouseMove:
        {
            mMousePosition = inEvent.mMousePosition;
            break;
        }
        }
    }

    glm::ivec2 InputManager::GetMousePosition() const
    {
        return mMousePosition;
    }

    glm::vec2 InputManager::GetAxis2D(EAxis2D axis) const
    {
        auto it = mAxisValues.find(axis);
        if(it != mAxisValues.end())
            return it->second;
        else
            return glm::vec2(0.0f, 0.0f);
    }
}
