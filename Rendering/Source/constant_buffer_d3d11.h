#ifndef MING3D_CONSTANTBUFFERD3D11_H
#define MING3D_CONSTANTBUFFERD3D11_H

#include "constant_buffer.h"
#include <d3d11.h>
#include <vector>

namespace Ming3D::Rendering
{
    class ConstantBufferD3D11 : public ConstantBuffer
    {
    public:
        ~ConstantBufferD3D11();

        ID3D11Buffer* mConstantBuffer;
        void* mConstantData = nullptr;
        size_t mSize;
    };
}
#endif // MING3D_CONSTANTBUFFERD3D11_H
