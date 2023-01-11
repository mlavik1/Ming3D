#include "widget_tree.h"
#include "widget.h"
#include "visual.h"
#include "gui_vertex_data.h"
#include <cassert>
#include <cstring>
#include <memory>
#include "Model/material.h"
#include "Model/material_factory.h"
#include "GameEngine/game_engine.h"
#include "render_device.h"
#include "graphics_data.h"
#include "gui_resource_manager.h"
#include "GameEngine/game_engine.h"

namespace Ming3D
{
    WidgetTree::WidgetTree()
    {
        mCanvasSize = glm::vec2(1280.0f, 720.0f);
        mRootSubmeshNode = nullptr;
        mRootWidget = std::make_shared<Widget>();
        WidgetTransform rootTransform{};
        rootTransform.anchorMin = rootTransform.anchorMax = glm::vec2(0.0f, 0.0f);
        rootTransform.mSize = mCanvasSize;
        mRootWidget->SetWidgetTransform(rootTransform);

        mMaterial = GGameEngine->GetGUIResourceManager()->GetDefaultGUIMaterial();
        mMeshBuffer = new MeshBuffer();

        Rendering::VertexLayout vertexLayout;
        vertexLayout.VertexComponents.push_back(Rendering::EVertexComponent::Position);
        vertexLayout.VertexComponents.push_back(Rendering::EVertexComponent::Colour);
        vertexLayout.VertexComponents.push_back(Rendering::EVertexComponent::TexCoord);
        mVertexData = std::make_unique<Rendering::VertexData>(vertexLayout, 0);
        mIndexData = std::make_unique<Rendering::IndexData>(0);
    }

    WidgetTree::~WidgetTree()
    {
    }

    void WidgetTree::UpdateWidgetRecursive(Widget* widget, WidgetUpdateParams params)
    {
        if (!widget->IsEnabled())
            return;

        params.mVisualsInvalidated |= widget->mWidgetInvalidated;
        widget->mWidgetInvalidated = false;

        WidgetRect widgetRect = ToScreenSpaceRect(widget->getAbsoluteRect());
        WidgetRect parentRect = params.mContentRect;

        // Calculate content rect (rect to render widget in) and visible rect (usually the same, unless parent widget is smaller)
        const glm::vec2 contentXYBounds = widgetRect.mPosition + widgetRect.mSize;
        const glm::vec2 parentXYBounds = parentRect.mPosition + parentRect.mSize;
        const float croppedPosX = std::fminf(std::fmaxf(widgetRect.mPosition.x, parentRect.mPosition.x), parentXYBounds.x);
        const float croppedPosY = std::fminf(std::fmaxf(widgetRect.mPosition.y, parentRect.mPosition.y), parentXYBounds.y);
        const glm::vec2 visibleXYBounds = glm::min(glm::min(contentXYBounds, parentXYBounds), params.mVisibleRect.mPosition + params.mVisibleRect.mSize);

        // Update contect rect
        params.mContentRect.mPosition = widgetRect.mPosition;
        params.mContentRect.mSize = widgetRect.mSize;
        // Update visible rect
        params.mVisibleRect.mPosition = glm::vec2(croppedPosX, croppedPosY);
        params.mVisibleRect.mSize = visibleXYBounds - params.mVisibleRect.mPosition;

        // Update visuals
        for (auto& visual : widget->mVisuals)
        {
            // Re-create vertex data of visual if it is invalidated
            if (params.mVisualsInvalidated || visual->mVisualInvalidated)
            {
                visual->RecreateMeshData(params.mVisibleRect);
                visual->mVisualInvalidated = false;
            }

            unsigned int vertCount = 0;
            unsigned int indCount = 0;

            // Get required mesh data size
            visual->GetMeshDataSize(vertCount, indCount);

            unsigned int totalVertCount = mVertexIndex + vertCount;
            unsigned int totalIndCount = mTriangleIndex + indCount;

            // Reallocate mesh data if needed -> TODO: Shrink?
            if (mVertexData->GetNumVertices() < totalVertCount)
                mVertexData->Resize(totalVertCount);
            if (mIndexData->GetNumIndices() < totalIndCount)
                mIndexData->Resize(totalIndCount);
            
            // Update submesh vertex/index data
            GUIVertexData* vertData = mVertexData->GetDataAs<GUIVertexData>();
            unsigned int* indData = mIndexData->GetData();
            visual->GetMeshData(&vertData[mVertexIndex], &indData[mTriangleIndex]);

            for (size_t i = 0; i < indCount; ++i)
                indData[mTriangleIndex + i] += mVertexIndex;

            // Add render batch
            RenderBatch batch;
            batch.mMaterial = visual->GetMaterial()->mMaterialBuffer;
            batch.mMeshBuffer = mMeshBuffer;
            batch.mModelMatrix = mTransformMatrix;
            batch.mStartIndex = mTriangleIndex;
            batch.mNumIndices = indCount;
            AddRenderBatch(batch);

            mVertexIndex += vertCount;
            mTriangleIndex += indCount;
        }

        // Update child widgets
        for (auto& childWidget : widget->mChildWidgets)
        {
            UpdateWidgetRecursive(childWidget.get(), params);
        }

        widget->mWidgetInvalidated = false;
    }

    void WidgetTree::AddRenderBatch(RenderBatch batch)
    {
        // Try to combine with previous batch
        bool combinedBatch = false;
        if (mRenderBatches.size() > 0)
        {
            RenderBatch& lastBatch = mRenderBatches[mRenderBatches.size() - 1];
            if (lastBatch.mMaterial == batch.mMaterial && lastBatch.mMeshBuffer == batch.mMeshBuffer
                && lastBatch.mStartIndex + lastBatch.mNumIndices == batch.mStartIndex)
            {
                // Combine batches
                lastBatch.mNumIndices += batch.mNumIndices;
                combinedBatch = true;
            }

        }
        if (!combinedBatch)
            mRenderBatches.push_back(batch);
    }

    void WidgetTree::TickWidgetsRecursive(Widget* widget, float deltaTime)
    {
        if (!widget->mHasTicked)
            widget->Start();
        widget->mHasTicked = true;

        widget->Tick(deltaTime);
        
        for (auto& childWidget : widget->mChildWidgets)
        {
            TickWidgetsRecursive(childWidget.get(), deltaTime);
        }
    }

    void WidgetTree::TickWidgets(float deltaTime)
    {
        TickWidgetsRecursive(mRootWidget.get(), deltaTime);
    }


    void WidgetTree::UpdateWidgetTree()
    {
        mRenderBatches.clear();
        mVertexIndex = 0;
        mTriangleIndex = 0;

        WidgetUpdateParams params;
        params.mContentRect.mPosition = glm::vec2(0.0f, 0.0f);
        params.mContentRect.mSize = mCanvasSize;
        params.mVisibleRect = params.mContentRect;
        params.mVisualsInvalidated = mVisualsInvalidated;

        UpdateWidgetRecursive(mRootWidget.get(), params);

        mRootWidget->mWidgetInvalidated = false;
        mVisualsInvalidated = false;
    }

    void WidgetTree::UpdateRenderData()
    {
        if (mMeshBuffer->mVertexBuffer == nullptr)
        {
            mMeshBuffer->mVertexBuffer = GGameEngine->GetRenderDevice()->CreateVertexBuffer(mVertexData.get(), Rendering::EBufferUsage::DynamicDraw);
            mMeshBuffer->mIndexBuffer = GGameEngine->GetRenderDevice()->CreateIndexBuffer(mIndexData.get(), Rendering::EBufferUsage::DynamicDraw);
        }
        else
        {
            GGameEngine->GetRenderDevice()->UpdateVertexBuffer(mMeshBuffer->mVertexBuffer, mVertexData.get());
            GGameEngine->GetRenderDevice()->UpdateIndexBuffer(mMeshBuffer->mIndexBuffer, mIndexData.get());
        }
    }

    WidgetRect WidgetTree::ToScreenSpaceRect(const WidgetRect& rect)
    {
        WidgetRect screenRect = rect;
        screenRect.mPosition.y = mCanvasSize.y - screenRect.mPosition.y - screenRect.mSize.y;
        return screenRect;
    }

    void WidgetTree::HandleEventRecursive(Widget* widget, InputEvent event, glm::ivec2 mousePosition)
    {
        const WidgetRect rect = widget->getAbsoluteRect();
        if (rect.Contains(mousePosition))
        {
            if (!widget->mHasMouseInside)
            {
                widget->mHasMouseInside = true;
                InputEvent mouseEnterEvent{};
                mouseEnterEvent.mType = InputEventType::MouseEnter;
                mouseEnterEvent.mMousePosition = event.mMousePosition;
                widget->OnInputEvent(mouseEnterEvent);
            }
            widget->OnInputEvent(event);
            for (auto child : widget->GetChildren())
            {
                HandleEventRecursive(child.get(), event, mousePosition);
            }
        }
        else if (widget->mHasMouseInside)
        {
            widget->mHasMouseInside = false;
            InputEvent mouseLeaveEvent{};
            mouseLeaveEvent.mType = InputEventType::MouseLeave;
            mouseLeaveEvent.mMousePosition = event.mMousePosition;
            widget->OnInputEvent(mouseLeaveEvent);
        }
    }

    void WidgetTree::HandleEvents(const std::vector<InputEvent>& events, glm::ivec2 mousePosition)
    {
        for (std::vector<InputEvent>::const_iterator iter = events.begin(); iter != events.end(); ++iter)
        {
            InputEvent event = (*iter);
            Widget* widget = mRootWidget.get();
            HandleEventRecursive(widget, event, mousePosition);
        }
    }

    void WidgetTree::SetWidget(std::shared_ptr<Widget> widget)
    {
        mRootWidget->mChildWidgets.clear(); // TODO
        mRootWidget->addWidget(widget);
        mVisualsInvalidated = true;
    }

    void WidgetTree::SetTransform(glm::mat4 transMat)
    {
        if (mTransformMatrix != transMat)
        {
            mTransformMatrix = transMat;
            mVisualsInvalidated = true;
        }
    }

    void WidgetTree::SetCanvasSize(glm::ivec2 canvasSize)
    {
        mCanvasSize = canvasSize;
        WidgetTransform rootTransform = mRootWidget->GetWidgetTransform();
        rootTransform.mSize = mCanvasSize;
        mRootWidget->SetWidgetTransform(rootTransform);
        mVisualsInvalidated = true;
    }

    size_t WidgetTree::GetNumBatches() const
    {
        return mRenderBatches.size();
    }

    void WidgetTree::GetBatch(size_t batchIndex, RenderBatch* outBatch)
    {
        *outBatch = mRenderBatches[batchIndex];
    }
}
