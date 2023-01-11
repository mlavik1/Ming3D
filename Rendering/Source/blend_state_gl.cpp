#ifdef MING3D_OPENGL
#include "blend_state_gl.h"

namespace Ming3D::Rendering
{
    BlendStateGL::BlendStateGL(bool enableBlend, EBlendMode blendMode)
        : BlendState(enableBlend, blendMode)
    {
        mBlendEnabled = enableBlend;

        switch (blendMode)
        {
        case EBlendMode::Zero:
        {
            mSrcFactor = GL_SRC_ALPHA;
            mDstFactor = GL_ZERO; 
            break;
        }
        case EBlendMode::One:
        {
            mSrcFactor = GL_SRC_ALPHA;
            mDstFactor = GL_ONE; 
            break;
        }
        case EBlendMode::OneMinusSrcAlpha:
        {
            mSrcFactor = GL_SRC_ALPHA;
            mDstFactor = GL_ONE_MINUS_SRC_ALPHA; 
            break;
        }
        }
    }
}
#endif
