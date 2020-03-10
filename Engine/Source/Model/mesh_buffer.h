#ifndef MING3D_MESHBUFFER_H
#define MING3D_MESHBUFFER_H

namespace Ming3D
{
    namespace Rendering
    {
        class VertexBuffer;
        class IndexBuffer;
        class TextureBuffer;
    }

    class MeshBuffer
    {
    public:
        Rendering::VertexBuffer* mVertexBuffer = nullptr;
        Rendering::IndexBuffer* mIndexBuffer = nullptr;
    };
}

#endif
