#pragma once

#include "glm/glm.hpp"
#include <vector>


#include "render_device.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader_program.h"
#include "sample_base.h"
#include <memory>

namespace Ming3D
{
    class RenderToTextureSample : public SampleBase
    {
    private:
        std::vector<ModelData*> mModels;

        std::shared_ptr<Rendering::RenderTarget> mRenderTarget;
        std::shared_ptr<Rendering::RenderTarget> mTextureRenderTarget;
        Rendering::RasteriserState* mOrthoRasterState;

    protected:
        virtual void init() override;
        virtual void tick() override;
    };
}
