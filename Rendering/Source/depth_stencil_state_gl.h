#ifndef MING3D_DEPTHSTENCILSTATEGL_H
#define MING3D_DEPTHSTENCILSTATEGL_H

#include "depth_stencil_state.h"
#include <GL/glew.h>

namespace Ming3D::Rendering
{
    class DepthStencilStateGL : public DepthStencilState
    {
    public:
        GLenum mDepthFunc = GL_LESS;
    };
}

#endif
