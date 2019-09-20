#ifndef MING3D_CAMERA_H
#define MING3D_CAMERA_H

#include "glm/glm.hpp"

namespace Ming3D
{
    class RenderTarget;
    class RenderPipelineParams;

    class Camera
    {
    public:
        Camera();
        ~Camera();

        glm::mat4 mCameraMatrix;
        RenderTarget* mRenderTarget = nullptr;
        RenderPipelineParams* mRenderPipelineParams = nullptr;
    };
}

#endif
