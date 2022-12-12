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

        virtual void Initialise() override;
        virtual void Update() override;

    private:
        void HandleKeyDown(WPARAM wParam, HWND hWnd);
        void HandleKeyUp(WPARAM wParam, HWND hWnd);
        void HandleMouseDown(int button, HWND hWnd);
        void HandleMouseUp(int button, HWND hWnd);
        void HandleMouseMove(HWND hWnd);
        void AddInputEvent(InputEvent event, HWND hWnd);
        KeyCode GetKeyCode(WPARAM wParam);
    };
}

#endif
