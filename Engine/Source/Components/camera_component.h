#ifndef MING3D_CAMERACOMPONENT_H
#define MING3D_CAMERACOMPONENT_H

#include "component.h"
#include "SceneRenderer/camera.h"

namespace Ming3D
{
    class CameraComponent : public Component
    {
        DEFINE_CLASS(Ming3D::CameraComponent, Ming3D::Component)

    private:
        static void InitialiseClass();

        Camera* mCamera = nullptr;

    protected:
        virtual void InitialTick() override;
        virtual void Tick(float inDeltaTime) override;

    public:
        CameraComponent();
        virtual ~CameraComponent();

        Camera* GetCamera() { return mCamera; }
    };
}

#endif
