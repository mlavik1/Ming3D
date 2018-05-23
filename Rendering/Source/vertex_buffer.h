#ifndef MING3D_VERTEX_BUFFER_H
#define MING3D_VERTEX_BUFFER_H

#include "graphics_data.h"

namespace Ming3D
{
    class VertexBuffer
    {
    private:
        VertexLayout mVertexLayout;
        size_t mVertexSize;

    public:
        void SetVertexLayout(const VertexLayout& inLayout);
        const VertexLayout& GetVertexLayout() { return mVertexLayout; }
        size_t GetVertexSize() { return mVertexSize; }
    };
}

#endif
