#ifdef MING3D_OPENGL
#include "index_buffer_gl.h"

namespace Ming3D::Rendering
{
    void IndexBufferGL::SetGLBuffer(GLuint inBuffer)
    {
        mGLBuffer = inBuffer;
    }

    GLuint IndexBufferGL::GetGLBuffer()
    {
        return mGLBuffer;
    }
}
#endif
