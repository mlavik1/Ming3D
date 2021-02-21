#pragma once

namespace Ming3D::Rendering
{
    /**
    * Enum specifying how a vertex buffer will be used.
    */
    enum class EBufferUsage
    {
        /* Static: Cannot be updated after initialisation. */
        StaticDraw,
        /* Dynamic: Can be updated at runtime. */
        DynamicDraw
    };
}
