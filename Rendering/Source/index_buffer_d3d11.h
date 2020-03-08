#ifndef MING3D_INDEX_BUFFER_D3D11_H
#define MING3D_INDEX_BUFFER_D3D11_H

#include "index_buffer.h"

#include <d3d11.h>

namespace Ming3D::Rendering
{
    class IndexBufferD3D11 : public IndexBuffer
    {
    private:
        ID3D11Buffer *mD3DBuffer;

    public:
        void SetD3DBuffer(ID3D11Buffer* inBuffer);
        ID3D11Buffer* GetD3DBuffer();
    };
}

#endif
