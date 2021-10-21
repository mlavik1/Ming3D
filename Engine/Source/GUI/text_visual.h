#pragma once
#include "visual.h"
#include "gui_vertex_data.h"
#include <vector>
#include <string>
#include "font_face.h"

namespace Ming3D
{
    class Material;

    class TextVisual : public Visual
    {
    private:
        std::vector<GUIVertexData> mVertexData;
        std::vector<unsigned int> mIndexData;

        std::wstring mText;
        glm::vec4 mColour;
        Material* mMaterial;
        FontFace* mFontFace;

    public:
        TextVisual();
        ~TextVisual();

        virtual void RecreateMeshData(const WidgetRect visibleRect) override;
        virtual void GetMeshDataSize(unsigned int& outVertexCount, unsigned int& outIndexCount) override;
        virtual void GetMeshData(GUIVertexData* outVertData, unsigned int* outIndexData) override;
        virtual Material* GetMaterial() override;

        void SetText(const std::wstring text);
        void SetFontFace(FontFace* fontFace);
        void SetColour(glm::vec4 colour);
    };
}
