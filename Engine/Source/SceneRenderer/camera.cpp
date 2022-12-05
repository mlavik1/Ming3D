#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "render_target.h"
#include "render_pipeline.h"
#include "forward_render_pipeline.h"
#include "Debug/st_assert.h"
#include "render_target.h"

namespace Ming3D
{
    Camera::Camera()
    {
        mRenderPipeline = new ForwardRenderPipeline(); // TODO: make adjustable
    }

    Camera::~Camera()
    {
        delete mRenderPipeline;
    }

    CameraViewport Camera::GetAbsoluteViewport() const
    {
        if (mRenderTarget == nullptr)
            return { 0.0f, 0.0f, 0.0f, 0.0f };
        const TextureInfo rtInfo = mRenderTarget->mTextureInfo;
        return { mViewport.x * rtInfo.mWidth, mViewport.y * rtInfo.mHeight, mViewport.width * rtInfo.mWidth, mViewport.height * rtInfo.mHeight };
    }
}
