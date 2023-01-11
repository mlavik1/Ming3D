#ifndef MING3D_TEXTURE_BUFFER_H
#define MING3D_TEXTURE_BUFFER_H

namespace Ming3D::Rendering
{
    /**
    * Base class for Texture Buffers.
    * A buffered texture on the GPU.
    */
    class TextureBuffer
    {
    public:
        virtual ~TextureBuffer() = default;
    };
}

#endif
