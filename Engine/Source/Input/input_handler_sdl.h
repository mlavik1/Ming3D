#ifndef MING3D_INPUTHANDLER_SDL_H
#define MING3D_INPUTHANDLER_SDL_H

#include "input_handler.h"
#include "input_event.h"
#include <SDL2/SDL.h>

namespace Ming3D
{
    class InputHandlerSDL : public InputHandler
    {
    public:
        virtual void Initialise() override;
        virtual void Update() override;

    private:
        glm::vec2 mLeftConrollerAxis;
        glm::vec2 mRightConrollerAxis;

        void HandleKeyDown(SDL_Keycode inKeycode);
        void HandleKeyUp(SDL_Keycode inKeycode);
        void HandleAxis2D(Uint8 axis, Sint16 value);
        KeyCode GetKeyCode(SDL_Keycode inKeycode);
    };
}

#endif
