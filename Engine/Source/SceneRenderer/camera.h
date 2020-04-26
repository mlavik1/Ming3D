#ifndef MING3D_CAMERA_H
#define MING3D_CAMERA_H

#include "glm/glm.hpp"

namespace Ming3D
{
    class RenderPipeline;
    class RenderPipelineParams;

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
        Rendering::RenderTarget* mRenderTarget = nullptr;
        RenderPipeline* mRenderPipeline;
        RenderPipelineParams* mRenderPipelineParams = nullptr;
    };
}

#endif
