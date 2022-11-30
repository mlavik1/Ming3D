#pragma once
#include <vector>
#include <memory>
#include "GUI/widget.h"

namespace Ming3D
{
    class TreeViewWidget;

    class SceneHierarchyWidget : Widget
    {
        DEFINE_CLASS(Ming3D::SceneHierarchyWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();
        bool isInitialisedHACK = false;

    private:
        std::shared_ptr<TreeViewWidget> mTreeView;

    public:
        SceneHierarchyWidget();
        virtual ~SceneHierarchyWidget();

        virtual void Tick(float inDeltaTime) override;
    };
}
