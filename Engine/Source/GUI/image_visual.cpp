#include "image_visual.h"
#include <memory>
#include "Texture/texture.h"
#include "Model/material.h"
#include "Model/material_factory.h"

namespace Ming3D
{
    ImageVisual::ImageVisual()
    {
        mColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        mMaterial = MaterialFactory::GetDefaultGUIMaterial();
        mHasCustomMat = false;
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

    Material* ImageVisual::GetMaterial()
    {
        return mMaterial;
    }

    void ImageVisual::SetColour(glm::vec4 colour)
    {
        mColour = colour;
        mVisualInvalidated = true;
    }

    void ImageVisual::SetTexture(std::shared_ptr<Texture> texture)
    {
        if (!mHasCustomMat)
        {
            // Create new material
            // TODO: Add support for using different textures in the same default GUI material
            mHasCustomMat = true;
            mMaterial = new Material(mMaterial);
        }

        mMaterial->SetTexture(0, texture);
        mVisualInvalidated = true;
    }

    void ImageVisual::SetCustomMaterial(Material* material)
    {
        mMaterial = material;
        mVisualInvalidated = true;
    }
}
