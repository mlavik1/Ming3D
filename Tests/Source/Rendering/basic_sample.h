#pragma once

#include <vector>


#include "render_device.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader_program.h"

#include "sample_base.h"

namespace Ming3D
{
    class BasicSample : public SampleBase
    {
    private:
        std::vector<ModelData*> mModels;

        Rendering::RenderTarget* mRenderTarget;

    protected:
        virtual void init() override;
        virtual void tick() override;
    };
}
