#pragma once
#include "blend_state.h"
#include "GL/glew.h"

namespace Ming3D::Rendering
{
    class BlendStateGL : public BlendState
    {
    public:
        BlendStateGL(bool enableBlend, EBlendMode blendMode);
        virtual ~BlendStateGL();

        bool mBlendEnabled = false;
        GLenum mSrcFactor = GL_SRC_ALPHA;
        GLenum mDstFactor = GL_ONE_MINUS_SRC_ALPHA;
    };
}
