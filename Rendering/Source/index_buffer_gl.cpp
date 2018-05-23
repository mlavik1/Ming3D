#include "index_buffer_gl.h"

namespace Ming3D
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
