#ifndef MING3D_MESH_H
#define MING3D_MESH_H

#include <vector>
#include <memory>
#include "graphics_data.h"

namespace Ming3D
{
    class Mesh
    {
    private:
        std::unique_ptr<Rendering::VertexData> mVertexData;
        std::unique_ptr<Rendering::IndexData> mIndexData;

    public:
        ~Mesh();

        Rendering::VertexData* GetVertexData() const { return mVertexData.get(); }
        Rendering::IndexData* GetIndexData() const { return mIndexData.get(); }

        void SetVertexData(std::unique_ptr<Rendering::VertexData> data) { mVertexData = std::move(data); }
        void SetIndexData(std::unique_ptr<Rendering::IndexData> data) { mIndexData = std::move(data); }
    };
}

#endif
