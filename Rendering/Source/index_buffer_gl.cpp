#ifdef MING3D_OPENGL
#include "index_buffer_gl.h"

namespace Ming3D::Rendering
{
    IndexBufferGL::IndexBufferGL(EBufferUsage usage)
        : IndexBuffer(usage)
    {

    }

    IndexBufferGL::~IndexBufferGL()
    {
        if(mAssigned)
            glDeleteBuffers(1, &mGLBuffer);
    }

    void IndexBufferGL::SetGLBuffer(GLuint inBuffer)
    {
        mGLBuffer = inBuffer;
        mAssigned = true;
    }

    GLuint IndexBufferGL::GetGLBuffer()
    {
        return mGLBuffer;
    }
}
#endif
