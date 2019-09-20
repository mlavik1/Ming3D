#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "render_target.h"
#include "render_pipeline.h"

namespace Ming3D
{
    Camera::Camera()
    {
        mRenderPipelineParams = new RenderPipelineParams();
    }

    Camera::~Camera()
    {
        delete mRenderPipelineParams;
    }
}
