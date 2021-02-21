#include "image_visual.h"
#include <memory>

namespace Ming3D
{
    ImageVisual::ImageVisual()
    {
        mColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    ImageVisual::~ImageVisual()
    {

    }

    void ImageVisual::RecreateMeshData(const WidgetRect visibleRect)
    {
        mVertexData.resize(4);
        mVertexData[0].mPosition = glm::vec3(visibleRect.mPosition.x, visibleRect.mPosition.y, 0.0f);
        mVertexData[1].mPosition = glm::vec3(visibleRect.mPosition.x, visibleRect.mPosition.y + visibleRect.mSize.y, 0.0f);
        mVertexData[2].mPosition = glm::vec3(visibleRect.mPosition.x + visibleRect.mSize.x, visibleRect.mPosition.y + visibleRect.mSize.y, 0.0f);
        mVertexData[3].mPosition = glm::vec3(visibleRect.mPosition.x + visibleRect.mSize.x, visibleRect.mPosition.y, 0.0f);
        mVertexData[0].mColour = mColour;
        mVertexData[1].mColour = mColour;
        mVertexData[2].mColour = mColour;
        mVertexData[3].mColour = mColour;
        mVertexData[0].mTexCoord = glm::vec2(0.0f, 0.0f);
        mVertexData[1].mTexCoord = glm::vec2(0.0f, 1.0f);
        mVertexData[2].mTexCoord = glm::vec2(1.0f, 1.0f);
        mVertexData[3].mTexCoord = glm::vec2(1.0f, 0.0f);
        mIndexData = { 0, 3, 1, 1, 3, 2 };
    }

    void ImageVisual::GetMeshDataSize(unsigned int& outVertexCount, unsigned int& outIndexCount)
    {
        outVertexCount = mVertexData.size();
        outIndexCount = mIndexData.size();
    }

    void ImageVisual::GetMeshData(GUIVertexData* outVertData, unsigned int* outIndexData)
    {
        size_t size = sizeof(GUIVertexData);
        memcpy(outVertData, mVertexData.data(), mVertexData.size() * sizeof(GUIVertexData));
        memcpy(outIndexData, mIndexData.data(), mIndexData.size() * sizeof(unsigned int));
    }

    void ImageVisual::SetColour(glm::vec4 colour)
    {
        mColour = colour;
        mVisualInvalidated = true;
    }
}
