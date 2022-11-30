#ifndef MING3D_CAMERACOMPONENT_H
#define MING3D_CAMERACOMPONENT_H

#include "component.h"

namespace Ming3D
{
    namespace Rendering
    {
        class RenderTarget;
    }

    class RenderWindowHandle;
    class Camera;

    class CameraComponent : public Component
    {
        DEFINE_CLASS(Ming3D::CameraComponent, Ming3D::Component)

    private:
        static void InitialiseClass();

        Camera* mCamera = nullptr;
        RenderWindowHandle* mRenderWindow = nullptr; // TODO: Use a RenderTargetHandle instead of storing render target direclty in Camera?

    protected:
        virtual void InitialTick() override;
        virtual void Tick(float inDeltaTime) override;

    public:
        CameraComponent();
        virtual ~CameraComponent();

        Camera* GetCamera() { return mCamera; }

        /* Set a RenderTarget that the Camera will render to. */
        void SetRenderTarget(Rendering::RenderTarget* target);

        /* Set a RenderWindow that the Camera will render to. */
        void SetRenderTarget(RenderWindowHandle* wndHandle);
    };
}

#endif
