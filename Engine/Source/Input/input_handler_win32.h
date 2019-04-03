#ifndef MING3D_INPUTHANDLER_WIN32_H
#define MING3D_INPUTHANDLER_WIN32_H

#include "input_handler.h"
#include <Windows.h>
#include "input_event.h"

namespace Ming3D
{
    class InputHandlerWin32 : public InputHandler
    {
    public:
        LRESULT HandleWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

        virtual void Update() override;

    private:
        void HandleKeyDown(WPARAM wParam);
        void HandleKeyUp(WPARAM wParam);
        KeyCode GetKeyCode(WPARAM wParam);
    };
}

#endif
