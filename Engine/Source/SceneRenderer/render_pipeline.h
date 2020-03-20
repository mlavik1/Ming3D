#ifndef MING3D_RENDERPIPELINE_H
#define MING3D_RENDERPIPELINE_H

#include <vector>
#include "render_scene_object.h"
#include "camera.h"

namespace Ming3D
{
    class RenderPipelineNode
    {
    public:
        MeshBuffer* mMesh = nullptr;
        MaterialBuffer* mMaterial = nullptr;
        glm::mat4 mModelMatrix;
    };

    class RenderPipelineNodeCollection
    {
        using nodes_t = std::vector<RenderPipelineNode*>;
    public:
        using iterator = nodes_t::iterator;

        ~RenderPipelineNodeCollection();

        void clear();
        RenderPipelineNode* push_back();
        iterator begin();
        iterator end();

    private:
        nodes_t mNodes;
        size_t mSize = 0;
    };

    struct RenderPipelineParams
    {
        Camera* mCamera = nullptr;
        glm::vec3 mMainLightDir;
        RenderPipelineNodeCollection mNodes;
    };

    class RenderPipeline
    {
    public:
        virtual ~RenderPipeline() {}
        virtual void Initialise() {}
        virtual void Render(RenderPipelineParams& params) = 0;
    };
}

#endif
