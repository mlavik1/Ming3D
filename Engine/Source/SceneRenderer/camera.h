#ifndef MING3D_CAMERA_H
#define MING3D_CAMERA_H

#include "glm/glm.hpp"

#include <memory>

namespace Ming3D
{
    class RenderPipeline;
    struct RenderPipelineParams;

    struct CameraViewport
    {
        float x = 0.0f, y = 0.0f, width = 1.0f, height = 1.0f;
    };

    namespace Rendering
    {
        class RenderTarget;
    }

    class Camera
    {
    public:
        Camera();
        ~Camera();

        glm::mat4 mCameraMatrix;
        glm::mat4 mProjectionMatrix;
        std::shared_ptr<Rendering::RenderTarget> mRenderTarget;
        glm::vec3 mPosition;
        RenderPipeline* mRenderPipeline; // TODO: unique_ptr
        CameraViewport mViewport;
        int mRenderOrder = 0;

        CameraViewport GetAbsoluteViewport() const;
    };
}

#endif
