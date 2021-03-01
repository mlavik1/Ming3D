#pragma once
#include "gui_vertex_data.h"
#include "widget_transform.h"

namespace Ming3D
{
    class Material;
    
    enum class VisualType
    {
        ImageVisual,
        TextVisual
    };

    class Visual
    {
        friend class WidgetTree;

    protected:
        bool mVisualInvalidated = true;
        VisualSubmeshNode* mSubmeshData = nullptr;
        WidgetTree* mWidgetTree;

    public:
        Visual() {}
        virtual ~Visual() {}

        virtual void RecreateMeshData(const WidgetRect visibleRect) = 0;
        virtual void GetMeshDataSize(unsigned int& outVertexCount, unsigned int& outIndexCount) = 0;
        virtual void GetMeshData(GUIVertexData* outVertData, unsigned int* outIndexData) = 0;
        virtual Material* GetMaterial() = 0;
    };
}
