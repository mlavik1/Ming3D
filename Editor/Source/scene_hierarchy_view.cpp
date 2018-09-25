#include "scene_hierarchy_view.h"

namespace Ming3D
{
	SceneHierarchyView::SceneHierarchyView(Control* arg_parent)
		: UserControl(arg_parent)
	{
        mTreeView = new NativeUI::TreeView(this);
        mTreeView->SetVerticalSizeMode(NativeUI::SizeMode::Relative);
        mTreeView->SetHorizontalSizeMode(NativeUI::SizeMode::Relative);
        mTreeView->SetSize(1.0f, 1.0f);

        //mTreeView->AddItem(NativeUI::TreeViewItem("TEST test", 42));
	}

    void SceneHierarchyView::AddActor(Actor* inActor)
    {
        mTreeView->AddItem(NativeUI::TreeViewItem(inActor->GetActorName(), (long)&inActor));
        mSelectedActor = inActor;
    }

    void SceneHierarchyView::RemoveActor(Actor* inActor)
    {
        // TODO
    }
}