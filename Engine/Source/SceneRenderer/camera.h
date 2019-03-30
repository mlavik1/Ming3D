#ifndef MING3D_CAMERA_H
#define MING3D_CAMERA_H

#include "glm/glm.hpp"

namespace Ming3D
{
    class RenderTarget;

    class Camera
    {
    public:
        Camera();

        glm::mat4 mCameraMatrix;
        RenderTarget* mRenderTarget = nullptr;
    };
}

#endif
