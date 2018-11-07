#ifndef MING3D_PROEPRTYINSPECTORBUILDER_H
#define MING3D_PROEPRTYINSPECTORBUILDER_H

#include <functional>
#include <vector>
#include "control.h"
#include "button.h"
#include "text_box.h"
#include "Object/property.h"
#include "Object/object.h"

namespace Ming3D
{
    class PropertyInspectorBuilder
    {
    private:
        NativeUI::Control* mParentControl;
        std::vector<NativeUI::Control*> mControls;

        float mCurrHeight = 0.0f;
        float mCurrWidth = 0.0f;

        const float ctrlHeight = 20.0f;
        const float vertPadding = 10.0f;

        bool mHorizontalMode = false;

        std::vector<NativeUI::Control*> mQueueHorizontalControls;

        NativeUI::Button* CreateButton(std::string inText, int inFontSize, float inCtrlHeight);
        NativeUI::TextBox* CreateTextBox(std::string inText, int inFontSize, float inCtrlHeight);
        NativeUI::TextBox* CreateFloatEditBox(int inFontSize, float inCtrlHeight, std::function<float()> inValueSetter, std::function<void(float)> inOnChangedCallback);

    public:
        PropertyInspectorBuilder(NativeUI::Control* inControl);

        void Text(std::string inText, int inTextSize = 14);
        void Space();
        void Button(std::string inText, std::function<void()> inCallbback);
        void PropertyField(Property* inProp, Object* inObject, std::string inName);

        void BeginHorizontal();
        void EndHorizontal();

        void Clear();
    };
}

#endif
