#ifndef MING3D_VERTEX_BUFFER_H
#define MING3D_VERTEX_BUFFER_H

#include "graphics_data.h"

namespace Ming3D::Rendering
{
    /**
     * Enum specifying how a vertex buffer will be used.
     */
    enum class EVertexBufferUsage
    {
        /* Static: Cannot be updated after initialisation. */
        StaticDraw,
        /* Dynamic: Can be updated at runtime. */
        DynamicDraw
    };

    /**
    * Vertex buffer base class.
    * There will be one sub-class for each rendering API.
    * Created by the RenderDevice.
    */
    class VertexBuffer
    {
    private:
        VertexLayout mVertexLayout;
        size_t mVertexSize = 0;
        EVertexBufferUsage mUsage = EVertexBufferUsage::StaticDraw;

    public:
        VertexBuffer(const VertexLayout& inLayout, EVertexBufferUsage usage);
        virtual ~VertexBuffer() {}

        const VertexLayout& GetVertexLayout() { return mVertexLayout; }
        size_t GetVertexSize() { return mVertexSize; }
        EVertexBufferUsage GetUsage() { return mUsage; }
    };
}

#endif
