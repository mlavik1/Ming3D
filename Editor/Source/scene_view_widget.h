#pragma once
#include "GUI/widget.h"
#include <memory>

namespace Ming3D
{
    class World;
    class Camera;

    class SceneViewWidget : public Widget
    {
        DEFINE_CLASS(Ming3D::SceneViewWidget, Ming3D::Widget)

    private:
        static void InitialiseClass();
        std::unique_ptr<Camera> mCamera;
        std::weak_ptr<World> mGameWorld;

    public:
        SceneViewWidget();
        virtual ~SceneViewWidget();

        virtual void Start() override;
        virtual void Tick(float inDeltaTime) override;
    };
}
