#pragma once
#include "visual.h"
#include "gui_vertex_data.h"
#include "font_face.h"
#include "text_properties.h"
#include <vector>
#include <string>

namespace Ming3D
{
    class Material;

    class TextVisual : public Visual
    {
    private:
        std::vector<GUIVertexData> mVertexData;
        std::vector<unsigned int> mIndexData;

        std::string mText;
        glm::vec4 mColour;
        std::shared_ptr<Material> mMaterial;
        FontFace* mFontFace;
        float mFontScale;
        EHorizontalAlignment mHorizontalAlignment;
        EVerticalAlignment mVerticalAlignment;
        int mMargin = 4;

    public:
        TextVisual();
        ~TextVisual();

        virtual void RecreateMeshData(const WidgetRect visibleRect) override;
        virtual void GetMeshDataSize(unsigned int& outVertexCount, unsigned int& outIndexCount) override;
        virtual void GetMeshData(GUIVertexData* outVertData, unsigned int* outIndexData) override;
        virtual std::shared_ptr<Material> GetMaterial() override;

        void SetText(const std::string& text);
        void SetFontFace(FontFace* fontFace);
        void SetColour(glm::vec4 colour);
        void SetFontScale(float scale);
        void SetHorizontalAlignment(EHorizontalAlignment alignment);
        void SetVerticalAlignment(EVerticalAlignment alignment);

        const float GetFontScale() const { return mFontScale; };
    };
}
