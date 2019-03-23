#ifndef MING3D_MESHBUFFER_H
#define MING3D_MESHBUFFER_H

namespace Ming3D
{
    class VertexBuffer;
    class IndexBuffer;
    class TextureBuffer;

    class MeshBuffer
    {
    public:
        VertexBuffer* mVertexBuffer = nullptr;
        IndexBuffer* mIndexBuffer = nullptr;
    };
}

#endif
