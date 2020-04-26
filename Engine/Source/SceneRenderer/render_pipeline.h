#ifndef MING3D_RENDERPIPELINE_H
#define MING3D_RENDERPIPELINE_H

#include <vector>
#include "render_scene_object.h"
#include "camera.h"
#include "Light/light_source.h"
#include "render_type.h"

namespace Ming3D
{
    class RenderScene;

    class RenderPipelineNode
    {
    public:
        MeshBuffer* mMesh = nullptr;
        MaterialBuffer* mMaterial = nullptr;
        glm::mat4 mModelMatrix;
        ERenderType mRenderType;
        float mSquareDistance;
    };

    class RenderPipelineNodeCollection
    {
        using nodes_t = std::vector<RenderPipelineNode*>;
        using iterator = nodes_t::iterator;

    private:
        nodes_t mNodes;
        size_t mSize = 0;

    public:
        ~RenderPipelineNodeCollection();

        void clear();
        RenderPipelineNode* push_back();
        RenderPipelineNode* at(size_t index);
        size_t size();
        iterator begin();
        iterator end();
    };

    struct RenderPipelineParams
    {
        Camera* mCamera;
        RenderPipelineNodeCollection mVisibleNodes;
        std::vector<unsigned int> mOpaqueNodeIndices;
        std::vector<unsigned int> mTransparentNodeIndices;
        bool mIsShadowPass = false;
    };

    struct RenderPipelineContext
    {
        Camera* mMainCamera;
        RenderScene* mScene;
        LightSource* mMainLight;
    };

    class RenderPipeline
    {
    public:
        virtual ~RenderPipeline() {}
        virtual void Render(const RenderPipelineContext& context, RenderPipelineParams& params) = 0;
    };
}

#endif
