#ifndef MING3D_PROEPRTYINSPECTOR_H
#define MING3D_PROEPRTYINSPECTOR_H

#include "user_control.h"
#include "Actors/actor.h"
#include <vector>

#include "text_box.h"
#include "button.h"
#include "window_layout_builder.h"

namespace Ming3D
{
    class PropertyInspector : public NativeUI::UserControl
    {
    private:
        Actor* mSelectedActor = nullptr;
        std::vector<Control*> mControls;

        bool mNeedsRefresh = false;

        void RefreshInternal();

        WindowLayoutBuilder* layoutBuilder;

        EditorControlStyle headerTextStyle;
        EditorControlStyle subHeaderTextStyle;

    public:
        PropertyInspector(Control* arg_parent);
        void SelectActor(Actor* inActor);
        void Refresh();
        void OnTick();
    };
}

#endif
