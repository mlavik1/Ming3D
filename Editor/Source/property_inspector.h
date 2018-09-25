#ifndef MING3D_PROEPRTYINSPECTOR_H
#define MING3D_PROEPRTYINSPECTOR_H

#include "user_control.h"
#include "Actors/actor.h"
#include <vector>

#include "text_box.h"
#include "button.h"

namespace Ming3D
{
    class PropertyInspector : public NativeUI::UserControl
    {
    private:
        Actor* mSelectedActor = nullptr;
        std::vector<Control*> mControls;

        NativeUI::TextBox* CreateTextBox(std::string inText, int inFontSize, float inCtrlHeight, float& outCurrHeight);
        NativeUI::Button* CreateButton(std::string inText, int inFontSize, float inCtrlHeight, float& outCurrHeight);

        bool mNeedsRefresh = false;

        void RefreshInternal();

    public:
        PropertyInspector(Control* arg_parent);
        void SelectActor(Actor* inActor);
        void Refresh();
        void OnTick();
    };
}

#endif
