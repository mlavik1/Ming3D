#pragma once
#include "gui_vertex_data.h"
#include "widget_transform.h"
#include <memory>

namespace Ming3D
{
    class Material;
    class WidgetTree;
    
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
        WidgetTree* mWidgetTree = nullptr;

    public:
        Visual() {}
        virtual ~Visual() {}

        virtual void RecreateMeshData(const WidgetRect visibleRect) = 0;
        virtual void GetMeshDataSize(unsigned int& outVertexCount, unsigned int& outIndexCount) = 0;
        virtual void GetMeshData(GUIVertexData* outVertData, unsigned int* outIndexData) = 0;
        virtual std::shared_ptr<Material> GetMaterial() = 0;
    };
}
