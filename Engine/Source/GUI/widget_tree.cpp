#include "widget_tree.h"
#include "widget.h"
#include "visual.h"
#include "gui_vertex_data.h"
#include <cassert>
#include <memory>
#include "Model/material.h"
#include "Model/material_factory.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "graphics_data.h"

namespace Ming3D
{
    WidgetTree::WidgetTree()
    {
        mRootSubmeshNode = nullptr;
        mRootWidget = new Widget();

        mMaterial = MaterialFactory::GetDefaultGUIMaterial();
        mMeshBuffer = new MeshBuffer();

        Rendering::VertexLayout vertexLayout;
        vertexLayout.VertexComponents.push_back(Rendering::EVertexComponent::Position);
        vertexLayout.VertexComponents.push_back(Rendering::EVertexComponent::Colour);
        vertexLayout.VertexComponents.push_back(Rendering::EVertexComponent::TexCoord);
        mVertexData = new Rendering::VertexData(vertexLayout, 0);
        mIndexData = new Rendering::IndexData(0);
    }

    WidgetTree::~WidgetTree()
    {
        delete mRootWidget;
    }

    void WidgetTree::UpdateWidgetRecursive(Widget* widget, WidgetUpdateParams params)
    {
        params.mVisualsInvalidated |= widget->mWidgetInvalidated;
        widget->mWidgetInvalidated = false;

        WidgetRect widgetRect = widget->getAbsoluteRect();
        WidgetRect parentRect = params.mContentRect;

        // Calculate content rect (rect to render widget in) and visible rect (usually the same, unless parent widget is smaller)
        const glm::vec2 contentXYBounds = widgetRect.mPosition + widgetRect.mSize;
        const glm::vec2 parentXYBounds = parentRect.mPosition + parentRect.mSize;
        const float visiblePosX = std::fminf(std::fmaxf(widgetRect.mPosition.x, parentRect.mPosition.x), parentXYBounds.x);
        const float visiblePosY = std::fminf(std::fmaxf(widgetRect.mPosition.y, parentRect.mPosition.y), parentXYBounds.y);
        const float visibleSizeW = std::fminf(contentXYBounds.x, parentXYBounds.x) - visiblePosX;
        const float visibleSizeH = std::fminf(contentXYBounds.y, parentXYBounds.y) - visiblePosY;

        // Update contect rect
        params.mContentRect.mPosition = widgetRect.mPosition;
        params.mContentRect.mSize = widgetRect.mSize;
        // Update visible rect
        params.mVisibleRect.mPosition = glm::vec2(visiblePosX, visiblePosY);
        params.mVisibleRect.mSize = glm::vec2(visibleSizeW, visibleSizeH);

        // Update visuals
        for (Visual* visual : widget->mVisuals)
        {
            if (params.mVisualsInvalidated || visual->mVisualInvalidated)
            {
                unsigned int vertCount = 0;
                unsigned int indCount = 0;

                visual->RecreateMeshData(params.mVisibleRect);

                // Get required mesh data size
                visual->GetMeshDataSize(vertCount, indCount);

                // Reallocate submesh data
                visual->mSubmeshData = ReallocSubmesh(visual->mSubmeshData, vertCount, indCount);

                // Update submesh vertex/index data
                GUIVertexData* vertData = mVertexData->GetDataAs<GUIVertexData>();
                unsigned int* indData = mIndexData->GetData();
                visual->GetMeshData(&vertData[visual->mSubmeshData->mVertexStartIndex], &indData[visual->mSubmeshData->mTriangleStartIndex]);

                const unsigned int indexOffset = visual->mSubmeshData->mVertexStartIndex;
                for (int iIndex = 0; iIndex < visual->mSubmeshData->mNumIndices; iIndex++)
                    indData[visual->mSubmeshData->mTriangleStartIndex + iIndex] += indexOffset;

                visual->mVisualInvalidated = false;
            }

            // Add render batch
            RenderBatch batch;
            batch.mMaterial = visual->GetMaterial()->mMaterialBuffer;
            batch.mMeshBuffer = mMeshBuffer;
            batch.mModelMatrix = mTransformMatrix;
            batch.mStartIndex = visual->mSubmeshData->mTriangleStartIndex;
            batch.mNumIndices = visual->mSubmeshData->mNumIndices;
            mRenderBatches.push_back(batch);
        }

        // Update child widgets
        for (Widget* childWidget : widget->mChildWidgets)
        {
            UpdateWidgetRecursive(childWidget, params);
        }

        widget->mWidgetInvalidated = false;
    }

    void WidgetTree::UpdateWidgetTree()
    {
        mRenderBatches.clear();

        WidgetUpdateParams params;
        WidgetRect rootWidgetRect = mRootWidget->getAbsoluteRect();
        params.mContentRect.mPosition = rootWidgetRect.mPosition;
        params.mContentRect.mSize = rootWidgetRect.mSize;
        params.mVisibleRect = params.mContentRect;
        params.mVisualsInvalidated = mRootWidget->mWidgetInvalidated;

        for (Widget* childWidget : mRootWidget->mChildWidgets)
        {
            UpdateWidgetRecursive(childWidget, params);
        }

        mRootWidget->mWidgetInvalidated = false;
    }

    void WidgetTree::UpdateRenderData()
    {
        if (mMeshBuffer->mVertexBuffer == nullptr)
        {
            mMeshBuffer->mVertexBuffer = GGameEngine->GetRenderDevice()->CreateVertexBuffer(mVertexData, Rendering::EBufferUsage::DynamicDraw);
            mMeshBuffer->mIndexBuffer = GGameEngine->GetRenderDevice()->CreateIndexBuffer(mIndexData, Rendering::EBufferUsage::DynamicDraw);
        }
        else
        {
            GGameEngine->GetRenderDevice()->UpdateVertexBuffer(mMeshBuffer->mVertexBuffer, mVertexData);
            GGameEngine->GetRenderDevice()->UpdateIndexBuffer(mMeshBuffer->mIndexBuffer, mIndexData);
        }
    }

    void WidgetTree::OffsetSubmeshes(VisualSubmeshNode* node, int64_t vertexOffset, int64_t indexOffset)
    {
        VisualSubmeshNode* currNode = node;
        while (currNode != nullptr)
        {
            int64_t newStartVertex = static_cast<int64_t>(currNode->mVertexStartIndex) + vertexOffset;
            int64_t newStartIndex = static_cast<int64_t>(currNode->mVertexStartIndex) + vertexOffset;

            // Move vertex/index data
            memmove(&mVertexData[newStartVertex], &mVertexData[node->mVertexStartIndex], currNode->mNumVertices);
            memmove(&mIndexData[newStartIndex], &mIndexData[node->mTriangleStartIndex], currNode->mNumIndices);

            // Set new start indices
            currNode->mVertexStartIndex = newStartVertex;
            currNode->mTriangleStartIndex = newStartIndex;

            currNode = currNode->mNextNode;
        }
    }

    VisualSubmeshNode* WidgetTree::ReallocSubmesh(VisualSubmeshNode* oldNode, unsigned int vertexCount, unsigned int indexCount)
    {
        VisualSubmeshNode* node = oldNode;

        if (mRootSubmeshNode == nullptr)
        {
            assert(oldNode == nullptr);

            mRootSubmeshNode = mTailSubmeshNode = new VisualSubmeshNode();
            mRootSubmeshNode->mNumVertices = vertexCount;
            mRootSubmeshNode->mNumIndices = indexCount;
            mRootSubmeshNode->mVertexStartIndex = 0;
            mRootSubmeshNode->mTriangleStartIndex = 0;
            node = mRootSubmeshNode;

            mVertexData->Resize(mVertexData->GetNumVertices() + vertexCount);
            mIndexData->Resize(mIndexData->GetNumIndices() + indexCount);
        }
        else if(oldNode != nullptr)
        {
            size_t oldVertCount = oldNode->mNumVertices;
            size_t oldIndCount = oldNode->mNumIndices;

            if (oldVertCount != vertexCount || oldIndCount != indexCount)
            {
                oldNode->mNumVertices = vertexCount;
                oldNode->mNumIndices = indexCount;
                node = oldNode;

                // Grow vertex data (before we offset other nodes)
                if(vertexCount > oldVertCount)
                    mVertexData->Resize(mVertexData->GetNumVertices() + (vertexCount - oldVertCount));
                if (indexCount > oldIndCount)
                    mIndexData->Resize(mIndexData->GetNumIndices() + (indexCount - oldIndCount));

                // Offset succeeding nodes
                OffsetSubmeshes(node->mNextNode, static_cast<int>(vertexCount) - static_cast<int>(oldVertCount), static_cast<int>(indexCount) - static_cast<int>(oldIndCount));
            
                // Shink vertex data
                if (vertexCount < oldVertCount)
                    mVertexData->Resize(mVertexData->GetNumVertices() - (oldVertCount - indexCount));
                if (indexCount < oldIndCount)
                    mIndexData->Resize(mIndexData->GetNumIndices() - (oldVertCount - indexCount));
            }
        }
        else
        {
            node = new VisualSubmeshNode();
            node->mNumVertices = vertexCount;
            node->mNumIndices = indexCount;
            node->mVertexStartIndex = mTailSubmeshNode->mVertexStartIndex + mTailSubmeshNode->mNumVertices;
            node->mTriangleStartIndex = mTailSubmeshNode->mTriangleStartIndex + mTailSubmeshNode->mNumIndices;
            mTailSubmeshNode->mNextNode = node;

            mVertexData->Resize(mVertexData->GetNumVertices() + vertexCount);
            mIndexData->Resize(mIndexData->GetNumIndices() + indexCount);
        }

        return node;
    }

    void WidgetTree::SetRootWidget(Widget* widget)
    {
        mRootWidget = widget;
    }

    void WidgetTree::SetTransform(glm::mat4 transMat)
    {
        mTransformMatrix = transMat;
    }

    size_t WidgetTree::GetNumBatches()
    {
        return mRenderBatches.size();
    }

    void WidgetTree::GetBatch(size_t batchIndex, RenderBatch* outBatch)
    {
        *outBatch = mRenderBatches[batchIndex];
    }
}