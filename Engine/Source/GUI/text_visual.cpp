#include "text_visual.h"
#include <memory>
#include "Texture/texture.h"
#include "Model/material.h"
#include "Model/material_factory.h"

namespace Ming3D
{
    TextVisual::TextVisual()
    {
        mColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        mMaterial = MaterialFactory::GetDefaultGUIMaterial();
        mFontFace = nullptr;
        mFontScale = 1.0f;
    }

    TextVisual::~TextVisual()
    {

    }

    void TextVisual::RecreateMeshData(const WidgetRect visibleRect)
    {
        mVertexData.resize(4 * mText.size());
        mIndexData.clear();
        mIndexData.reserve(6 * mText.size());

        const glm::vec2 startOrigin(visibleRect.mPosition.x, visibleRect.mPosition.y + visibleRect.mSize.y * 0.5f /* temp hack for centering text */);
        
        // Text origin
        glm::vec2 currOrigin = startOrigin;

        for (int iChar = 0; iChar < mText.size(); iChar++)
        {
            // Read glyph
            const wchar_t currChar = mText[iChar];
            FontGlyph glyph;
            if (!mFontFace->GetGlyph(currChar, glyph))
                continue;
            
            // Calculate position and size
            const float x = currOrigin.x + glyph.mBearingX * mFontScale;
            const float y = currOrigin.y - (glyph.mHeight - glyph.mBearingY) * mFontScale;
            const float w = glyph.mWidth * mFontScale;
            const float h = glyph.mHeight * mFontScale;

            // Invert Y
            glyph.mTexCoord = glm::vec2(glyph.mTexCoord.x, 1.0f - glyph.mTexCoord.y);

            const size_t iVert = iChar * 4;
            mVertexData[iVert + 0].mPosition = glm::vec3(x, y, 0.0f);
            mVertexData[iVert + 1].mPosition = glm::vec3(x, y + h, 0.0f);
            mVertexData[iVert + 2].mPosition = glm::vec3(x + w, y + h, 0.0f);
            mVertexData[iVert + 3].mPosition = glm::vec3(x + w, y, 0.0f);
            mVertexData[iVert + 0].mColour = mColour;
            mVertexData[iVert + 1].mColour = mColour;
            mVertexData[iVert + 2].mColour = mColour;
            mVertexData[iVert + 3].mColour = mColour;
            mVertexData[iVert + 0].mTexCoord = glyph.mTexCoord + glm::vec2(0.0f, -glyph.mTexSize.y);
            mVertexData[iVert + 1].mTexCoord = glyph.mTexCoord + glm::vec2(0.0f, 0.0f);
            mVertexData[iVert + 2].mTexCoord = glyph.mTexCoord + glm::vec2(glyph.mTexSize.x, 0.0f);
            mVertexData[iVert + 3].mTexCoord = glyph.mTexCoord + glm::vec2(glyph.mTexSize.x, -glyph.mTexSize.y);

            // Advance to next character
            currOrigin.x += glyph.mAdvance * mFontScale;

            mIndexData.push_back(iVert + 0);
            mIndexData.push_back(iVert + 3);
            mIndexData.push_back(iVert + 1);
            mIndexData.push_back(iVert + 1);
            mIndexData.push_back(iVert + 3);
            mIndexData.push_back(iVert + 2);
        }
    }

    void TextVisual::GetMeshDataSize(unsigned int& outVertexCount, unsigned int& outIndexCount)
    {
        outVertexCount = mVertexData.size();
        outIndexCount = mIndexData.size();
    }

    void TextVisual::GetMeshData(GUIVertexData* outVertData, unsigned int* outIndexData)
    {
        size_t size = sizeof(GUIVertexData);
        memcpy(outVertData, mVertexData.data(), mVertexData.size() * sizeof(GUIVertexData));
        memcpy(outIndexData, mIndexData.data(), mIndexData.size() * sizeof(unsigned int));
    }

    Material* TextVisual::GetMaterial()
    {
        return mMaterial;
    }

    void TextVisual::SetText(const std::wstring text)
    {
        mText = text;
        mVisualInvalidated = true;
    }

    void TextVisual::SetFontFace(FontFace* fontFace)
    {
        mFontFace = fontFace;
        mMaterial = fontFace->mMaterial;
        mVisualInvalidated = true;
    }

    void TextVisual::SetColour(glm::vec4 colour)
    {
        mColour = colour;
        mVisualInvalidated = true;
    }

    void TextVisual::SetFontScale(float scale)
    {
        mFontScale = scale;
        mVisualInvalidated = true;
    }
}
