#ifndef SAMPLE_BASE_H
#define SAMPLE_BASE_H

#include "render_device.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader_program.h"

namespace Ming3D
{
    class SampleBase
    {
    public:

        class Vertex
        {
        public:
            glm::vec3 mVertex;
            glm::vec3 mNormal;
            glm::vec2 mTexCoord;
        };

        class MeshData
        {
        public:
            std::vector<Vertex> mVertices;
            std::vector<unsigned int> mIndices;
            bool mHasNormals = false;
            bool mHasTexCoords = false;

            VertexBuffer* mVertexBuffer;
            IndexBuffer* mIndexBuffer;
            Texture* mTexture;
            TextureBuffer* mTextureBuffer;
        };

        class ModelData
        {
        public:
            std::vector<MeshData*> mMeshes;
            ShaderProgram* mShaderProgram = nullptr;
            glm::vec3 mPosition;
        };

    public:
        void RunSample();

    protected:
        WindowBase* mMainWindow;
        RenderDevice* mRenderDevice;
        RenderWindow* mRenderWindow;

        virtual void init();
        virtual void tick();

        ModelData* LoadModel(const char* inModel);
        MeshData* CreateRectangleMesh(float inWidth, float inHeight);
    };
}

#endif
