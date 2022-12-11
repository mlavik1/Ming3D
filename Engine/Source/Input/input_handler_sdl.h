#ifndef MING3D_INPUTHANDLER_SDL_H
#define MING3D_INPUTHANDLER_SDL_H

#include "input_handler.h"
#include "input_event.h"
#include <SDL2/SDL.h>

namespace Ming3D
{
    namespace Rendering
    {
        class SDLWindow;
    }

    class InputHandlerSDL : public InputHandler
    {
    public:
        InputHandlerSDL(Rendering::SDLWindow* window);
        virtual ~InputHandlerSDL();
        virtual void Initialise() override;
        virtual void Update() override;

    private:
        Rendering::SDLWindow* mWindow;
        glm::vec2 mLeftConrollerAxis;
        glm::vec2 mRightConrollerAxis;
        glm::ivec2 mMousePosition;

        void HandleMouseDown(Uint8 mouseButton);
        void HandleMouseUp(Uint8 mouseButton);
        void HandleKeyDown(SDL_Keycode inKeycode);
        void HandleKeyUp(SDL_Keycode inKeycode);
        void HandleControllerButtonDown(Uint8 button);
        void HandleControllerButtonUp(Uint8 button);
        void HandleAxis2D(Uint8 axis, Sint16 value);
        KeyCode GetKeyCode(SDL_Keycode inKeycode);
        KeyCode GetControllerKeyCode(Uint8 keycode);
        int GetMouseButton(Uint8 buttonId);
        void AddInputEvent(InputEvent event);
    };
}

#endif
