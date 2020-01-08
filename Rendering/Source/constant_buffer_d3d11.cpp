#include "constant_buffer_d3d11.h"

namespace Ming3D
{
    ConstantBufferD3D11::~ConstantBufferD3D11()
    {
        if (mConstantData != nullptr)
        {
            delete mConstantData;
        }
    }
}
