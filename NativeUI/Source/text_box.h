#ifndef NATIVEUI_TEXTBOX_H
#define NATIVEUI_TEXTBOX_H

#include "control.h"
#include <unordered_map>
#include <functional>

namespace NativeUI
{
	class TextBox : public Control
	{
    private:
        static WNDCLASSEX* ControlClass;
        static std::unordered_map<void*, TextBox*> TextBoxInstances;
        HWND mEditHWND;

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        static void CreateControlClass();

        std::vector<std::function<void(std::string)>> mTextChangedCallbacks;

        bool mReadOnly = true;
        std::string mText = "";

    protected:
        virtual void OnTransformUpdated() override;
        
	public:
        TextBox(Control* arg_parent);

        void SetReadonly(bool arg_readonly);
		void SetText(std::string arg_text);
        void SetFontSize(int arg_size);

        std::string GetText() const;

        void RegisterTextChangedCallback(std::function<void(std::string)> inCallback);

	};
}

#endif
