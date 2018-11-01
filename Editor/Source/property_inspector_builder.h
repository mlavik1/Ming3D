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

        const float ctrlHeight = 20.0f;
        const float vertPadding = 10.0f;

        NativeUI::Button* CreateButton(std::string inText, int inFontSize, float inCtrlHeight, float& outCurrHeight);
        NativeUI::TextBox* CreateTextBox(std::string inText, int inFontSize, float inCtrlHeight, float& outCurrHeight);

    public:
        PropertyInspectorBuilder(NativeUI::Control* inControl);

        void Text(std::string inText, int inTextSize = 12);
        void Space();
        void Button(std::string inText, std::function<void()> inCallbback);
        void PropertyField(Property* inProp, Object* inObject, std::string inName);

        void Clear();
    };
}

#endif
