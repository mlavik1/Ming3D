#pragma once

#include <memory>

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
        std::unique_ptr<Rendering::VertexBuffer> mVertexBuffer;
        std::unique_ptr<Rendering::IndexBuffer> mIndexBuffer;

        MeshBuffer() = default;
        MeshBuffer(const MeshBuffer&) = delete;
        MeshBuffer& operator=(const MeshBuffer&) = delete;
    };
}
