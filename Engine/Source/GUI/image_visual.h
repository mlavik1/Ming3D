#pragma once
#include "visual.h"
#include "gui_vertex_data.h"
#include <vector>

namespace Ming3D
{
    class ImageVisual : public Visual
    {
    private:
        std::vector<GUIVertexData> mVertexData;
        std::vector<unsigned int> mIndexData;

        glm::vec4 mColour;

    public:
        ImageVisual();
        ~ImageVisual();

        virtual void RecreateMeshData(const WidgetRect visibleRect) override;
        virtual void GetMeshDataSize(unsigned int& outVertexCount, unsigned int& outIndexCount) override;
        virtual void GetMeshData(GUIVertexData* outVertData, unsigned int* outIndexData) override;

        void SetColour(glm::vec4 colour);
    };
}
