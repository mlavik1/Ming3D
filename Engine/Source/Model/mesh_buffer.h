#ifndef MING3D_MESHBUFFER_H
#define MING3D_MESHBUFFER_H

namespace Ming3D
{
    class MeshBuffer
    {
    public:
        VertexBuffer* mVertexBuffer;
        IndexBuffer* mIndexBuffer;
        TextureBuffer* mTextureBuffer;
    };
}

#endif
