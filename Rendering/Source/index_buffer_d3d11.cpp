#ifdef MING3D_D3D11
#include "index_buffer_d3d11.h"

namespace Ming3D
{
    void IndexBufferD3D11::SetD3DBuffer(ID3D11Buffer* inBuffer)
    {
        mD3DBuffer = inBuffer;
    }

    ID3D11Buffer* IndexBufferD3D11::GetD3DBuffer()
    {
        return mD3DBuffer;
    }
}
#endif

