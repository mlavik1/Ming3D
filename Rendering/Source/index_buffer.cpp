#include "index_buffer.h"

namespace Ming3D::Rendering
{
    IndexBuffer::IndexBuffer(EBufferUsage usage)
    {
        mUsage = usage;
    }

    void IndexBuffer::SetNumIndices(unsigned int inNumIndices)
    {
        mNumIndices = inNumIndices;
    }

    unsigned int IndexBuffer::GetNumIndices()
    {
        return mNumIndices;
    }
}
