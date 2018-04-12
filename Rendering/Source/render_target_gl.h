#ifndef MING3D_RENDER_TARGET_GL_H
#define MING3D_RENDER_TARGET_GL_H

#include "render_target.h"

namespace Ming3D
{
	class RenderTargetGL : public RenderTarget
	{
	public:
		RenderTargetGL(WindowBase* inWindow);

		virtual void BeginRendering() override;
		virtual void EndRendering() override;
	};
}

#endif
