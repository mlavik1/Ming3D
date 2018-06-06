#ifndef MING3D_VERTEX_BUFFER_H
#define MING3D_VERTEX_BUFFER_H

#include "graphics_data.h"

namespace Ming3D
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
        size_t mVertexSize;

    public:
        virtual ~VertexBuffer() {}
        void SetVertexLayout(const VertexLayout& inLayout);
        const VertexLayout& GetVertexLayout() { return mVertexLayout; }
        size_t GetVertexSize() { return mVertexSize; }
    };
}

#endif
