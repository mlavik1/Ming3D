#ifndef MING3D_VERTEX_BUFFER_D3D11_H
#define MING3D_VERTEX_BUFFER_D3D11_H

#include "vertex_buffer.h"

#include <d3d11.h>

namespace Ming3D
{
    class VertexBufferD3D11 : public VertexBuffer
    {
    private:
        ID3D11Buffer* mD3DBuffer = nullptr;

    public:
        virtual ~VertexBufferD3D11();
        void SetD3DBuffer(ID3D11Buffer* inBuffer);

        ID3D11Buffer* GetD3DBuffer();
    };
}

#endif
