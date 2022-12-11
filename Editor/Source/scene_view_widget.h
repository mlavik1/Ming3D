#pragma once
#include "GUI/widget.h"
#include <memory>
#include "glm/vec2.hpp"
#include "glm/gtc/quaternion.hpp"

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
        glm::vec2 mPrevMousePos;
        glm::vec3 mPosition;
        glm::mat4 mRotationMatrix;
        float mPitch = 0.0f;
        float mYaw = 0.0f;
        bool mMousePressed = false;
        glm::vec2 mRotationDelta;
        const float mRotationSpeed = 150.0f;
        const float mMovementSpeed = 5.0f;

    public:
        SceneViewWidget();
        virtual ~SceneViewWidget();

        virtual void Start() override;
        virtual void Tick(float inDeltaTime) override;
        virtual void OnInputEvent(InputEvent event) override;
    };
}
