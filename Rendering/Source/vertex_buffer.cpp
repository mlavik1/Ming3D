#include "vertex_buffer.h"

namespace Ming3D::Rendering
{
    VertexBuffer::VertexBuffer(const VertexLayout& inLayout, EBufferUsage usage)
    {
        mUsage = usage;
        mVertexLayout = inLayout;
        for (const EVertexComponent& comp : mVertexLayout.VertexComponents)
        {
            mVertexSize += VertexData::GetVertexComponentSize(comp);
        }
    }
}
