#include "text_visual.h"
#include <memory>
#include <algorithm>
#include "Texture/texture.h"
#include "Model/material.h"
#include "Model/material_factory.h"
#include "gui_resource_manager.h"
#include "GameEngine/game_engine.h"

namespace Ming3D
{
    TextVisual::TextVisual()
    {
        mColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        mMaterial = GGameEngine->GetGUIResourceManager()->GetDefaultGUIMaterial();
        mFontFace = nullptr;
        mFontScale = 1.0f;
        mHorizontalAlignment = EHorizontalAlignment::Left;
        mVerticalAlignment = EVerticalAlignment::Bottom;
    }

    TextVisual::~TextVisual()
    {

    }

    void TextVisual::RecreateMeshData(const WidgetRect visibleRect)
    {
        mVertexData.resize(4 * mText.size());
        mIndexData.clear();
        mIndexData.reserve(6 * mText.size());

        const glm::vec2 min = glm::vec2(visibleRect.mPosition.x + mMargin, visibleRect.mPosition.y);
        const glm::vec2 max = visibleRect.mPosition + visibleRect.mSize - glm::vec2(mMargin, 0.0f);
        glm::vec2 startOrigin = min;

        // Vertical alignment
        if (mVerticalAlignment == EVerticalAlignment::Bottom)
            startOrigin += glm::vec2(0.0f, mFontFace->mMaxBearingOffset * mFontScale);
        else if (mVerticalAlignment == EVerticalAlignment::Centre)
            startOrigin += glm::vec2(0.0f, visibleRect.mSize.y * 0.5f - mFontFace->mMeanBearing * mFontScale * 0.5f);
        else if (mVerticalAlignment == EVerticalAlignment::Top)
            startOrigin += glm::vec2(0.0f, visibleRect.mSize.y - mFontFace->mFontSize * mFontScale);

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

            // Calculate start/end vertices of plyph quad
            const glm::vec2 glyphMin(std::min(x, max.x), std::min(y, max.y));
            const glm::vec2 glyphMax(std::min(x + w, max.x), std::min(y + h, max.y));

            if (glyphMax.x <= glyphMin.x)
                break;

            // Invert Y
            glyph.mTexCoord = glm::vec2(glyph.mTexCoord.x, 1.0f - glyph.mTexCoord.y);

            const size_t iVert = iChar * 4;
            mVertexData[iVert + 0].mPosition = glm::vec3(glyphMin.x, glyphMin.y, 0.0f);
            mVertexData[iVert + 1].mPosition = glm::vec3(glyphMin.x, glyphMax.y, 0.0f);
            mVertexData[iVert + 2].mPosition = glm::vec3(glyphMax.x, glyphMax.y, 0.0f);
            mVertexData[iVert + 3].mPosition = glm::vec3(glyphMax.x, glyphMin.y, 0.0f);
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

        if (mHorizontalAlignment != EHorizontalAlignment::Left)
        {
            float vertOffset = 0.0f;
            if (mHorizontalAlignment == EHorizontalAlignment::Centre)
                vertOffset = (visibleRect.mSize.x - (currOrigin.x - startOrigin.x)) * 0.5f;
            else if(mHorizontalAlignment == EHorizontalAlignment::Right)
                vertOffset = (visibleRect.mSize.x - (currOrigin.x - startOrigin.x));

            for (int iVert = 0; iVert < mText.size() * 4; iVert++)
                mVertexData[iVert].mPosition.x += vertOffset;
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

    void TextVisual::SetText(const std::string text)
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

    void TextVisual::SetHorizontalAlignment(EHorizontalAlignment alignment)
    {
        mHorizontalAlignment = alignment;
        mVisualInvalidated = true;
    }

    void TextVisual::SetVerticalAlignment(EVerticalAlignment alignment)
    {
        mVerticalAlignment = alignment;
        mVisualInvalidated = true;
    }
}
