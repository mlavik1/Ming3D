#ifndef MING3D_INPUTHANDLER_SDL_H
#define MING3D_INPUTHANDLER_SDL_H

#include "input_handler.h"
#include "input_event.h"
#include <SDL.h>

namespace Ming3D
{
    class InputHandlerSDL : public InputHandler
    {
    public:
        virtual void Update() override;

    private:
        void HandleKeyDown(SDL_Keycode inKeycode);
        void HandleKeyUp(SDL_Keycode inKeycode);
        KeyCode GetKeyCode(SDL_Keycode inKeycode);
    };
}

#endif
