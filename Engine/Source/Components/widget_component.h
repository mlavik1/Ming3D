#pragma once

#include <memory>
#include "component.h"
#include "GUI/widget_render_object.h"
#include "glm/vec2.hpp"

namespace Ming3D
{
    class Widget;

    enum class EWidgetRenderMode
    {
        World,
        Overlay
    };

    class WidgetComponent : public Component
    {
        DEFINE_CLASS(Ming3D::WidgetComponent, Ming3D::Component)

    private:
        static void InitialiseClass();

        WidgetRenderObject* mRenderObject = nullptr;
        std::shared_ptr<Widget> mWidget = nullptr;
        WidgetTree* mWidgetTree = nullptr;
        EWidgetRenderMode mRenderMode = EWidgetRenderMode::World;
        glm::ivec2 mCanvasSize = glm::ivec2(1280, 720);

    public:
        WidgetComponent();
        ~WidgetComponent();
        virtual void InitialiseComponent();

        virtual void Tick(float inDeltaTime) override;

        void SetWidget(std::shared_ptr<Widget> widget);
        void SetRenderMode(EWidgetRenderMode renderMode);
        void SetCanvasSize(glm::ivec2 canvasSize);
    };
}
