#ifndef MING3D_VERTEX_BUFFER_GL_H
#define MING3D_VERTEX_BUFFER_GL_H

#include "vertex_buffer.h"

#include <gl/glew.h>

namespace Ming3D
{
	class VertexBufferGL : public VertexBuffer
	{
	private:
		GLuint mGLBuffer;

	public:
		void SetGLBuffer(GLuint inBuffer);
		GLuint GetGLBuffer();
	};
}

#endif
