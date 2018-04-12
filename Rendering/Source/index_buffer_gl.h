#ifndef MING3D_INDEX_BUFFER_GL_H
#define MING3D_INDEX_BUFFER_GL_H

#include "index_buffer.h"

#include <gl/glew.h>

namespace Ming3D
{
	class IndexBufferGL : public IndexBuffer
	{
	private:
		GLuint mGLBuffer;

	public:
		void SetGLBuffer(GLuint inBuffer);
		GLuint GetGLBuffer();
	};
}

#endif
