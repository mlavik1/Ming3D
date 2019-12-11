#include "graphics_data.h"
#include <cstring>

namespace Ming3D
{
    VertexData::VertexData()
    {

    }

    VertexData::VertexData(std::vector<EVertexComponent> inComponents, size_t inNumVertices)
    {
        mVertexLayout.VertexComponents = inComponents;
        mVertexSize = 0;
        for (const EVertexComponent& comp : mVertexLayout.VertexComponents)
        {
            mVertexSize += GetVertexComponentSize(comp);
        }
        mData.resize(inNumVertices * mVertexSize);
    }

    VertexData::VertexData(VertexLayout inLayout, size_t inNumVertices)
        : VertexData(inLayout.VertexComponents, inNumVertices)
    {
    }

    VertexData::VertexData(const VertexData& other)
    {
        mData.resize(other.mData.size());
        memcpy(&mData[0], &other.mData[0], other.mData.size());
        mVertexLayout = other.mVertexLayout;
        mVertexSize = other.mVertexSize;
    }

    size_t VertexData::GetComponentOffset(EVertexComponent inComponent)
    {
        size_t offset = 0;
        for (const EVertexComponent& comp : mVertexLayout.VertexComponents)
        {
            if (comp == inComponent)
            {
                return offset;
            }
            else
                offset += GetVertexComponentSize(comp);
        }
        return -1;
    }

    void VertexData::GetComponentOffsets(EVertexComponent inComponent, std::vector<size_t>& outOffsets)
    {
        size_t offset = 0;
        for (const EVertexComponent& comp : mVertexLayout.VertexComponents)
        {
            if (comp == inComponent)
            {
                outOffsets.push_back(offset);
            }
            offset += GetVertexComponentSize(comp);
        }
    }

    size_t VertexData::GetNumVertices()
    {
        return mVertexSize == 0 ? 0 : mData.size() / mVertexSize;
    }

    size_t VertexData::GetVertexSize()
    {
        return mVertexSize;
    }

    size_t VertexData::GetVertexComponentSize(EVertexComponent inComp)
    {
        switch (inComp)
        {
        case EVertexComponent::Position:
            return sizeof(float) * 3;
            break;
        case EVertexComponent::Normal:
            return sizeof(float) * 3;
            break;
        case EVertexComponent::TexCoord:
            return sizeof(float) * 2;
            break;
        case EVertexComponent::Colour:
            return sizeof(float) * 4;
            break;
        }
        return 0;
    }


    IndexData::IndexData(size_t inNumIndices)
    {
        mData.resize(inNumIndices);
    }

    size_t IndexData::GetNumIndices()
    {
        return mData.size();
    }
}
