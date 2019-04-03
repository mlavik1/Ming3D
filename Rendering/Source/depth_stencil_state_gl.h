#ifndef MING3D_DEPTHSTENCILSTATEGL_H
#define MING3D_DEPTHSTENCILSTATEGL_H

#include "depth_stencil_state.h"
#include <gl/glew.h>

namespace Ming3D
{
    class DepthStencilStateGL : public DepthStencilState
    {
    public:
        GLenum mDepthFunc = GL_LESS;
    };
}

#endif
