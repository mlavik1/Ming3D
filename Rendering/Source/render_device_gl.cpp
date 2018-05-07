#include "render_device_gl.h"

#include "vertex_buffer_gl.h"
#include "index_buffer_gl.h"
#include "render_target_gl.h"
#include "shader_program_gl.h"
#include "texture_gl.h"

#include "Debug/debug.h"
#include "Debug/st_assert.h"

namespace Ming3D
{
	RenderDeviceGL::RenderDeviceGL()
	{
		if (glewInit())
		{
			LOG_ERROR() << "Failed to initialise GLEW";
		}
	}

	RenderDeviceGL::~RenderDeviceGL()
	{

	}

	RenderTarget* RenderDeviceGL::CreateRenderTarget(WindowBase* inWindow)
	{
		return new RenderTargetGL(inWindow);
	}

	VertexBuffer* RenderDeviceGL::CreateVertexBuffer(VertexData* inVertexData)
	{
		VertexBufferGL* vertexBuffer = new VertexBufferGL();
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, inVertexData->GetNumVertices() * inVertexData->GetVertexSize(), inVertexData->GetDataPtr(), GL_STATIC_DRAW);
		vertexBuffer->SetGLBuffer(vbo);
		vertexBuffer->SetVertexLayout(inVertexData->GetVertexLayout());
		return vertexBuffer;
	}

	IndexBuffer* RenderDeviceGL::CreateIndexBuffer(IndexData* inIndexData)
	{
		IndexBufferGL* indexBuffer = new IndexBufferGL();
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, inIndexData->GetNumIndices() * sizeof(unsigned int), inIndexData->GetData(), GL_STATIC_DRAW);
		indexBuffer->SetGLBuffer(ibo);
		indexBuffer->SetNumIndices(inIndexData->GetNumIndices());
		return indexBuffer;
	}

	ShaderProgram* RenderDeviceGL::CreateShaderProgram(const std::string& inVertexShaderCode, const std::string& inFragmentShaderCode, const ShaderProgramConstructionInfo& inConstructionInfo)
	{
		GLuint program = glCreateProgram();
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

		const char* str_v = inVertexShaderCode.c_str();
		const char* str_f = inFragmentShaderCode.c_str();

		glShaderSource(vs, 1, &str_v, 0);
		glShaderSource(fs, 1, &str_f, 0);

		glCompileShader(vs);
		glCompileShader(fs);
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);

		int vstatus, fstatus, lstatus;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &vstatus);
		printf("Vertex shader compile status: %s\n", (vstatus == GL_TRUE) ? "true" : "false");
		glGetShaderiv(fs, GL_COMPILE_STATUS, &fstatus);
		printf("Fragment shader compile status: %s\n", (fstatus == GL_TRUE) ? "true" : "false");
		glGetProgramiv(program, GL_LINK_STATUS, &lstatus);
		printf("Program link status: %s\n", (lstatus == GL_TRUE) ? "true" : "false");

		ShaderProgramGL* shaderProgram = new ShaderProgramGL();
		shaderProgram->SetGLProgram(program);
		shaderProgram->SetGLVertexShader(vs);
		shaderProgram->SetGLFragmentShader(fs);
		return shaderProgram;
	}

    void RenderDeviceGL::SetTexture(Texture* inTexture)
    {
        glEnable(GL_TEXTURE_2D); // TODO
        TextureGL* glTexture = (TextureGL*)inTexture;
        glActiveTexture(GL_TEXTURE0); // TODO: support multiple textures
        glBindTexture(GL_TEXTURE_2D, glTexture->GetGLTexture());
    }

	void RenderDeviceGL::SetRenderTarget(RenderTarget* inTarget)
	{
		mRenderTarget = (RenderTargetGL*)inTarget;
	}

	void RenderDeviceGL::SetActiveShaderProgram(ShaderProgram* inProgram)
	{
		mActiveShaderProgram = (ShaderProgramGL*)inProgram;
		if (mActiveShaderProgram != nullptr)
		{
			glUseProgram(mActiveShaderProgram->GetGLProgram());
		}
	}

	void RenderDeviceGL::BeginRendering()
	{
		// TODO: Do in render target
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	void RenderDeviceGL::EndRendering()
	{
	
	}

	void RenderDeviceGL::RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer)
	{
		VertexBufferGL* vertexBufferGL = (VertexBufferGL*)inVertexBuffer;
		IndexBufferGL* indexBufferGL = (IndexBufferGL*)inIndexBuffer;
        
        size_t vertexComponentIndex = 0;
        size_t vertexComponentOffset = 0;
        for (EVertexComponent vertexComponent : inVertexBuffer->GetVertexLayout().VertexComponents)
        {
            const size_t vertexComponentSize = VertexData::GetVertexComponentSize(vertexComponent);
            glEnableVertexAttribArray(vertexComponentIndex);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferGL->GetGLBuffer());
            glVertexAttribPointer(vertexComponentIndex, 3, GL_FLOAT, GL_FALSE, vertexBufferGL->GetVertexSize(), (void*)vertexComponentOffset);
            vertexComponentIndex++;
            vertexComponentOffset += vertexComponentSize;
        }

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferGL->GetGLBuffer());
		glDrawElements(GL_TRIANGLES, indexBufferGL->GetNumIndices(), GL_UNSIGNED_INT, 0);
	}

	void RenderDeviceGL::SetShaderUniformMat4x4(const char* inName, const glm::mat4 inMat)
	{
		GLuint MatrixID = glGetUniformLocation(mActiveShaderProgram->GetGLProgram(), inName);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &inMat[0][0]);
	}

	void RenderDeviceGL::SetShaderUniformVec4(const char* inName, const glm::vec4 inVec)
	{

	}
}
