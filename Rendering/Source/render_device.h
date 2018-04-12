#ifndef MING3D_RENDER_DEVICE_H
#define MING3D_RENDER_DEVICE_H
#include "render_target.h"

#include "window_base.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "graphics_data.h"
#include "shader_program.h"
#include "graphics_data.h"

#include <string>

namespace Ming3D
{
	class RenderDevice
	{
	public:
		virtual RenderTarget* CreateRenderTarget(WindowBase* inWindow) = 0;
		virtual VertexBuffer* CreateVertexBuffer(VertexData* inVertexData) = 0;
		virtual IndexBuffer* CreateIndexBuffer(IndexData* inIndexData) = 0;
		virtual ShaderProgram* CreateShaderProgram(const std::string& inVertexShaderCode, const std::string& inFragmentShaderCode, const ShaderProgramConstructionInfo& inConstructionInfo) = 0;
		virtual void SetRenderTarget(RenderTarget* inTarget) = 0;
		virtual void SetActiveShaderProgram(ShaderProgram* inProgram) = 0;
		virtual void BeginRendering() = 0;
		virtual void EndRendering() = 0;
		virtual void RenderPrimitive(VertexBuffer* inVertexBuffer, IndexBuffer* inIndexBuffer) = 0;
		//virtual void RenderMeshBatch(MeshRenderBatch* inBatch);

		virtual void SetShaderUniformMat4x4(const char* inName, const glm::mat4 inMat) = 0;
		virtual void SetShaderUniformVec4(const char* inName, const glm::vec4 inVec) = 0;
	};
}
#endif
