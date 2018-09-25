#ifndef MING3D_SCENE_HIERARCHY_VIEW_H
#define MING3D_SCENE_HIERARCHY_VIEW_H

#include "user_control.h"
#include "tree_view.h"
#include "Actors/actor.h"

namespace Ming3D
{
	class SceneHierarchyView : public NativeUI::UserControl
	{
    private:
        NativeUI::TreeView* mTreeView;
        Actor* mSelectedActor = nullptr;

	public:
        SceneHierarchyView(Control* arg_parent);
        void AddActor(Actor* inActor);
        void RemoveActor(Actor* inActor);
	};
}

#endif
