#pragma once

#include "render_device.h"

namespace Ming3D::Rendering
{
    class RenderDeviceFactory
    {
    public:
        static RenderDevice* CreateRenderDevice();
    };
}

