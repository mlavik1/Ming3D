#ifndef MING3D_CONSTANTBUFFERGL_H
#define MING3D_CONSTANTBUFFERGL_H

#include "constant_buffer.h"
#include <GL/glew.h>

namespace Ming3D::Rendering
{
    class ConstantBufferGL : public ConstantBuffer
    {
    public:
        GLuint mGLBuffer = 0;
    };
}
#endif // MING3D_CONSTANTBUFFERGL_H
