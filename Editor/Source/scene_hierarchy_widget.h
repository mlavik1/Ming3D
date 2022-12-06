#pragma once
#include <vector>
#include <memory>
#include "GUI/widget.h"

namespace Ming3D
{
    class TreeViewWidget;
    class Actor;

    class SceneHierarchyWidget : Widget
    {
        DEFINE_CLASS(Ming3D::SceneHierarchyWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();

    private:
        std::shared_ptr<TreeViewWidget> mTreeView;

        void AddActorRecursive(Actor* actor, int depth);
        void OnItemSelected(int id);

    public:
        SceneHierarchyWidget();
        virtual ~SceneHierarchyWidget();

        virtual void Start() override;
        virtual void Tick(float inDeltaTime) override;
    };
}
