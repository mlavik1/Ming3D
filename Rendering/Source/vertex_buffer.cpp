#include "vertex_buffer.h"

namespace Ming3D::Rendering
{
    void VertexBuffer::SetVertexLayout(const VertexLayout& inLayout)
    {
        mVertexLayout = inLayout;
        for (const EVertexComponent& comp : mVertexLayout.VertexComponents)
        {
            mVertexSize += VertexData::GetVertexComponentSize(comp);
        }
    }
}
