#ifdef MING3D_D3D11
#include "vertex_buffer_d3d11.h"

namespace Ming3D::Rendering
{
    VertexBufferD3D11::VertexBufferD3D11(const VertexLayout& inLayout, EVertexBufferUsage usage)
    : VertexBuffer(inLayout, usage)
    {

    }

    VertexBufferD3D11::~VertexBufferD3D11()
    {
        if (mD3DBuffer != nullptr)
        {
            mD3DBuffer->Release();
            delete mD3DBuffer;
        }
    }

    void VertexBufferD3D11::SetD3DBuffer(ID3D11Buffer* inBuffer)
    {
        mD3DBuffer = inBuffer;
    }

    ID3D11Buffer* VertexBufferD3D11::GetD3DBuffer()
    {
        return mD3DBuffer;
    }
}
#endif

