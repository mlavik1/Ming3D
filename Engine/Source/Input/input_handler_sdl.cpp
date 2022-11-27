#include "input_handler_sdl.h"
#include "GameEngine/game_engine.h"
#include "input_manager.h"
#include "SDL2/SDL_gamecontroller.h"
#include "sdl_window.h"

namespace Ming3D
{
    InputHandlerSDL::InputHandlerSDL(Rendering::SDLWindow* window)
    {
        mWindow = window;
    }

    InputHandlerSDL::~InputHandlerSDL()
    {

    }

    void InputHandlerSDL::Initialise()
    {
        SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

        if(SDL_IsGameController(0))
            SDL_GameControllerOpen(0);
    }

    void InputHandlerSDL::Update()
    {
        // TODO: Do this on init
        //if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 1)
	    //    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

        int mouseX = 0;
        int mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);
        glm::ivec2 newMousePos = glm::ivec2(mouseX, mWindow->GetHeight() - mouseY);
        if (newMousePos != mMousePosition)
        {
            mMousePosition = newMousePos;
            InputEvent inputEvent{};
            inputEvent.mType = InputEventType::MouseMove;
            inputEvent.mMousePosition = mMousePosition;
            AddInputEvent(inputEvent);
        }

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent))
        {
            switch (sdlEvent.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                HandleMouseDown(sdlEvent.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                HandleMouseUp(sdlEvent.button.button);
                break;
            case SDL_KEYDOWN:
                HandleKeyDown(sdlEvent.key.keysym.sym);
                break;
            case SDL_KEYUP:
                HandleKeyUp(sdlEvent.key.keysym.sym);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                HandleControllerButtonDown(sdlEvent.cbutton.button);
                break;
            case SDL_CONTROLLERBUTTONUP:
                HandleControllerButtonUp(sdlEvent.cbutton.button);
                break;
            case SDL_CONTROLLERAXISMOTION:
                HandleAxis2D(sdlEvent.caxis.axis, sdlEvent.caxis.value);
                break;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE)
                    mWindow->Close();
                break;
            }
        }
    }

    void InputHandlerSDL::HandleMouseDown(Uint8 mouseButton)
    {
        InputEvent inputEvent{};
        inputEvent.mType = InputEventType::MouseButtonDown;
        inputEvent.mMouseButton.mButton = static_cast<int>(mouseButton);
        AddInputEvent(inputEvent);
    }

    void InputHandlerSDL::HandleMouseUp(Uint8 mouseButton)
    {
        InputEvent inputEvent{};
        inputEvent.mType = InputEventType::MouseButtonUp;
        inputEvent.mMouseButton.mButton = static_cast<int>(mouseButton);
        AddInputEvent(inputEvent);
    }

    void InputHandlerSDL::HandleKeyDown(SDL_Keycode inKeycode)
    {
        InputEvent inputEvent{};
        inputEvent.mType = InputEventType::KeyDown;
        inputEvent.mKey.mKeyCode = GetKeyCode(inKeycode);
        if (inputEvent.mKey.mKeyCode != KeyCode::None)
            AddInputEvent(inputEvent);
    }

    void InputHandlerSDL::HandleKeyUp(SDL_Keycode inKeycode)
    {
        InputEvent inputEvent{};
        inputEvent.mType = InputEventType::KeyUp;
        inputEvent.mKey.mKeyCode = GetKeyCode(inKeycode);
        if (inputEvent.mKey.mKeyCode != KeyCode::None)
            AddInputEvent(inputEvent);
    }

    void InputHandlerSDL::HandleControllerButtonDown(Uint8 button)
    {
        InputEvent inputEvent{};
        inputEvent.mType = InputEventType::KeyDown;
        inputEvent.mKey.mKeyCode = GetControllerKeyCode(button);
        if (inputEvent.mKey.mKeyCode != KeyCode::None)
            AddInputEvent(inputEvent);
    }

    void InputHandlerSDL::HandleControllerButtonUp(Uint8 button)
    {
        InputEvent inputEvent{};
        inputEvent.mType = InputEventType::KeyUp;
        inputEvent.mKey.mKeyCode = GetControllerKeyCode(button);
        if (inputEvent.mKey.mKeyCode != KeyCode::None)
            AddInputEvent(inputEvent);
    }    

    void InputHandlerSDL::HandleAxis2D(Uint8 axis, Sint16 value)
    {
        InputEvent inputEvent{};
        inputEvent.mType = InputEventType::Axis2D;
        if(axis == SDL_CONTROLLER_AXIS_LEFTX)
        {
            mLeftConrollerAxis.x = ((static_cast<int>(value) + 32768) / 65535.0f) * 2.0f - 1.0f;
            inputEvent.mAxis.mAxis = EAxis2D::ControllerAxisLeft;
            inputEvent.mAxis.mValue = mLeftConrollerAxis;
            AddInputEvent(inputEvent);
        }
        else if(axis == SDL_CONTROLLER_AXIS_LEFTY)
        {
            mLeftConrollerAxis.y = ((static_cast<int>(value) + 32768) / 65535.0f) * 2.0f - 1.0f;
            inputEvent.mAxis.mAxis = EAxis2D::ControllerAxisLeft;
            inputEvent.mAxis.mValue = mLeftConrollerAxis;
            AddInputEvent(inputEvent);
        }
        else if(axis == SDL_CONTROLLER_AXIS_RIGHTX)
        {
            mRightConrollerAxis.x = ((static_cast<int>(value) + 32768) / 65535.0f) * 2.0f - 1.0f;
            inputEvent.mAxis.mAxis = EAxis2D::ControllerAxisRight;
            inputEvent.mAxis.mValue = mRightConrollerAxis;
            AddInputEvent(inputEvent);
        }
        else if(axis == SDL_CONTROLLER_AXIS_RIGHTY)
        {
            mRightConrollerAxis.y = ((static_cast<int>(value) + 32768) / 65535.0f) * 2.0f - 1.0f;
            inputEvent.mAxis.mAxis = EAxis2D::ControllerAxisRight;
            inputEvent.mAxis.mValue = mRightConrollerAxis;
            AddInputEvent(inputEvent);
        }
    }

    KeyCode InputHandlerSDL::GetKeyCode(SDL_Keycode inKeycode)
    {
        switch (inKeycode)
        {
        case SDLK_LCTRL: return KeyCode::Key_Ctrl;
        case SDLK_RCTRL: return KeyCode::Key_Ctrl;
        case SDLK_MENU: return KeyCode::Key_Win;
        case SDLK_LSHIFT: return KeyCode::Key_Shift;
        case SDLK_RSHIFT: return KeyCode::Key_Shift;
        case SDLK_RETURN: return KeyCode::Key_Enter;
        case SDLK_SPACE: return KeyCode::Key_Space;
        case SDLK_UP: return KeyCode::Key_Up;
        case SDLK_DOWN: return KeyCode::Key_Down;
        case SDLK_RIGHT: return KeyCode::Key_Right;
        case SDLK_LEFT: return KeyCode::Key_Left;
        case SDLK_0: return KeyCode::Key_0;
        case SDLK_1: return KeyCode::Key_1;
        case SDLK_2: return KeyCode::Key_2;
        case SDLK_3: return KeyCode::Key_3;
        case SDLK_4: return KeyCode::Key_4;
        case SDLK_5: return KeyCode::Key_5;
        case SDLK_6: return KeyCode::Key_6;
        case SDLK_7: return KeyCode::Key_7;
        case SDLK_8: return KeyCode::Key_8;
        case SDLK_9: return KeyCode::Key_9;
        case SDLK_q: return KeyCode::Key_Q;
        case SDLK_w: return KeyCode::Key_W;
        case SDLK_e: return KeyCode::Key_E;
        case SDLK_r: return KeyCode::Key_R;
        case SDLK_t: return KeyCode::Key_T;
        case SDLK_y: return KeyCode::Key_Y;
        case SDLK_u: return KeyCode::Key_U;
        case SDLK_i: return KeyCode::Key_I;
        case SDLK_o: return KeyCode::Key_O;
        case SDLK_p: return KeyCode::Key_P;
        case SDLK_a: return KeyCode::Key_A;
        case SDLK_s: return KeyCode::Key_S;
        case SDLK_d: return KeyCode::Key_D;
        case SDLK_f: return KeyCode::Key_F;
        case SDLK_g: return KeyCode::Key_G;
        case SDLK_h: return KeyCode::Key_H;
        case SDLK_j: return KeyCode::Key_J;
        case SDLK_k: return KeyCode::Key_K;
        case SDLK_l: return KeyCode::Key_L;
        case SDLK_z: return KeyCode::Key_Z;
        case SDLK_x: return KeyCode::Key_X;
        case SDLK_c: return KeyCode::Key_C;
        case SDLK_v: return KeyCode::Key_V;
        case SDLK_b: return KeyCode::Key_B;
        case SDLK_n: return KeyCode::Key_N;
        case SDLK_m: return KeyCode::Key_M;
        default:
            return KeyCode::None;
        }
    }

    KeyCode InputHandlerSDL::GetControllerKeyCode(Uint8 keycode)
    {
        switch (keycode)
        {
        case SDL_CONTROLLER_BUTTON_A: return KeyCode::Pad_A;
        case SDL_CONTROLLER_BUTTON_B: return KeyCode::Pad_B;
        case SDL_CONTROLLER_BUTTON_X: return KeyCode::Pad_X;
        case SDL_CONTROLLER_BUTTON_Y: return KeyCode::Pad_Y;
        default:
            return KeyCode::None;
        }
    }

    void InputHandlerSDL::AddInputEvent(InputEvent event)
    {
        event.mMousePosition = mMousePosition;
        GGameEngine->GetInputManager()->AddInputEvent(event);
    }
}
