#ifndef MING3D_CAMERA_H
#define MING3D_CAMERA_H

#include "glm/glm.hpp"
#include "render_target.h"

namespace Ming3D
{
    class Camera
    {
    public:
        Camera();

        glm::mat4 mCameraMatrix;
        RenderTarget* mRenderTarget = nullptr;
    };
}

#endif
