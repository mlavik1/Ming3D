#include "render_device_factory.h"

#ifdef MING3D_OPENGL
#include "render_device_gl.h"
#endif

#ifdef MING3D_D3D11
#include "render_device_d3d11.h"
#endif

namespace Ming3D::Rendering
{
    RenderDevice* RenderDeviceFactory::CreateRenderDevice()
    {
        #ifdef MING3D_OPENGL
            return new RenderDeviceGL();
        #endif

        #ifdef MING3D_D3D11
            return new RenderDeviceD3D11();
        #endif

        return nullptr;
    }
}