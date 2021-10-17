#ifdef _WIN32
#include "input_handler_win32.h"
#include "GameEngine/game_engine.h"
#include "input_manager.h"

namespace Ming3D
{
    void InputHandlerWin32::Initialise()
    {

    }

    void InputHandlerWin32::Update()
    {

    }

    LRESULT InputHandlerWin32::HandleWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_KEYDOWN:
            HandleKeyDown(wParam);
            break;
        case WM_KEYUP:
            HandleKeyUp(wParam);
            break;
        }

        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    void InputHandlerWin32::HandleKeyDown(WPARAM wParam)
    {
        InputEvent inputEvent;
        inputEvent.mType = InputEventType::KeyDown;
        inputEvent.mKey.mKeyCode = GetKeyCode(wParam);
        if(inputEvent.mKey.mKeyCode != KeyCode::None)
            GGameEngine->GetInputManager()->AddInputEvent(inputEvent);
    }

    void InputHandlerWin32::HandleKeyUp(WPARAM wParam)
    {
        InputEvent inputEvent;
        inputEvent.mType = InputEventType::KeyUp;
        inputEvent.mKey.mKeyCode = GetKeyCode(wParam);
        if (inputEvent.mKey.mKeyCode != KeyCode::None)
            GGameEngine->GetInputManager()->AddInputEvent(inputEvent);
    }

    KeyCode InputHandlerWin32::GetKeyCode(WPARAM wParam)
    {
        switch (wParam)
        {
        case VK_CONTROL: return KeyCode::Key_Ctrl;
        case VK_MENU: return KeyCode::Key_Win;
        case VK_SHIFT: return KeyCode::Key_Shift;
        case VK_RETURN: return KeyCode::Key_Enter;
        case VK_SPACE: return KeyCode::Key_Space;
        case VK_UP: return KeyCode::Key_Up;
        case VK_DOWN: return KeyCode::Key_Down;
        case VK_RIGHT: return KeyCode::Key_Right;
        case VK_LEFT: return KeyCode::Key_Left;
        case '1': return KeyCode::Key_1;
        case '2': return KeyCode::Key_2;
        case '3': return KeyCode::Key_3;
        case '4': return KeyCode::Key_4;
        case '5': return KeyCode::Key_5;
        case '6': return KeyCode::Key_6;
        case '7': return KeyCode::Key_7;
        case '8': return KeyCode::Key_8;
        case '9': return KeyCode::Key_9;
        case '0': return KeyCode::Key_0;
        case 'Q': return KeyCode::Key_Q;
        case 'W': return KeyCode::Key_W;
        case 'E': return KeyCode::Key_E;
        case 'R': return KeyCode::Key_R;
        case 'T': return KeyCode::Key_T;
        case 'Y': return KeyCode::Key_Y;
        case 'U': return KeyCode::Key_U;
        case 'I': return KeyCode::Key_I;
        case 'O': return KeyCode::Key_O;
        case 'P': return KeyCode::Key_P;
        case 'A': return KeyCode::Key_A;
        case 'S': return KeyCode::Key_S;
        case 'D': return KeyCode::Key_D;
        case 'F': return KeyCode::Key_F;
        case 'G': return KeyCode::Key_G;
        case 'H': return KeyCode::Key_H;
        case 'J': return KeyCode::Key_J;
        case 'K': return KeyCode::Key_K;
        case 'L': return KeyCode::Key_L;
        case 'Z': return KeyCode::Key_Z;
        case 'X': return KeyCode::Key_X;
        case 'C': return KeyCode::Key_C;
        case 'V': return KeyCode::Key_V;
        case 'B': return KeyCode::Key_B;
        case 'N': return KeyCode::Key_N;
        case 'M': return KeyCode::Key_M;
        default:
            return KeyCode::None;
        }
    }
}
#endif
