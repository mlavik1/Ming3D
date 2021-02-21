#ifndef MING3D_VERTEX_BUFFER_H
#define MING3D_VERTEX_BUFFER_H

#include "graphics_data.h"
#include "buffer_common.h"

namespace Ming3D::Rendering
{
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
        EBufferUsage mUsage = EBufferUsage::StaticDraw;

    public:
        VertexBuffer(const VertexLayout& inLayout, EBufferUsage usage);
        virtual ~VertexBuffer() {}

        const VertexLayout& GetVertexLayout() { return mVertexLayout; }
        size_t GetVertexSize() { return mVertexSize; }
        EBufferUsage GetUsage() { return mUsage; }
    };
}

#endif
