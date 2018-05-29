#pragma once

#include <Windows.h>
#include <gl/glew.h>

#include "glm/glm.hpp"
#include <vector>


#include "render_device.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader_program.h"

namespace Ming3D
{
    class RenderingTest
    {
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
        };

        class ModelData
        {
        public:
            std::vector<MeshData*> mMeshes;
            ShaderProgram* mShaderProgram = nullptr;
            glm::vec3 mPosition;
        };

    private:
        std::vector<ModelData*> mModels;
        RenderDevice* mRenderDevice;

    public:
        void Test();

        ModelData* LoadModel(const char* inModel);
    };
}
