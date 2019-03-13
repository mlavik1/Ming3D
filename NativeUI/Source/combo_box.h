#ifndef NATIVEUI_COMBOBOX_H
#define NATIVEUI_COMBOBOX_H

#include "control.h"
#include <unordered_map>

namespace NativeUI
{
    class ComboBox : public Control
    {
    private:
        static WNDCLASSEX* ControlClass;
        static std::unordered_map<void*, ComboBox*> ComboBoxInstances;

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static void CreateControlClass();

    public:
        ComboBox(Control* arg_parent);
    };
}

#endif
