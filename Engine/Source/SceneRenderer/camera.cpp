#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "render_target.h"
#include "render_pipeline.h"
#include "forward_render_pipeline.h"

namespace Ming3D
{
    Camera::Camera()
    {
        mRenderPipelineParams = new RenderPipelineParams();
        mRenderPipeline = new ForwardRenderPipeline(); // TODO: make adjustable
    }

    Camera::~Camera()
    {
        delete mRenderPipelineParams;
        delete mRenderPipeline;
    }
}
