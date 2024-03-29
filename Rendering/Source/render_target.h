#ifndef MING3D_RENDER_TARGET_H
#define MING3D_RENDER_TARGET_H

#include "window_base.h"

#include "texture_buffer.h"
#include "texture_info.h"

namespace Ming3D::Rendering
{
    class RenderWindow;

    /**
    * RenderTarget base class.
    * The RenderDevice will render to this.
    */
    class RenderTarget
    {
    public:
        RenderTarget();
        virtual ~RenderTarget() = default;

        TextureInfo mTextureInfo;

        /*
        * Returns the TextureBuffer for the coloiur attachment at the specified location.
        * This can be used as a texture when rendering.
        * @params inSlot  Colour texture buffer index (0, if single-target rendering)
        */
        virtual TextureBuffer* GetColourTextureBuffer(int inSlot) = 0;

        virtual TextureBuffer* GetDepthTextureBuffer() = 0;

        virtual RenderWindow* GetRenderWindow() = 0;
    };
}
#endif
