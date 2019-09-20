#include "render_pipeline.h"

namespace Ming3D
{
    RenderPipelineNodeCollection::~RenderPipelineNodeCollection()
    {
        for (size_t i = 0; i < mNodes.size(); i++)
        {
            delete mNodes[i];
        }
        mNodes.clear();
    }

    void RenderPipelineNodeCollection::clear()
    {
        mSize = 0;
    }

    RenderPipelineNode* RenderPipelineNodeCollection::push_back()
    {
        size_t index = mSize;
        mSize++;
        if (mSize > mNodes.size())
        {
            mNodes.push_back(new RenderPipelineNode());
        }

        return mNodes[index];
    }

    RenderPipelineNodeCollection::iterator RenderPipelineNodeCollection::begin()
    {
        return mNodes.begin();

    }
    RenderPipelineNodeCollection::iterator RenderPipelineNodeCollection::end()
    {
        return mNodes.begin() + mSize;
    }
}
