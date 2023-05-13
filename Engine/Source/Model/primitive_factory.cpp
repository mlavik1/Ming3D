#include "primitive_factory.h"
#include "mesh.h"
#include "glm/gtx/rotate_vector.hpp"
#include <cstring>
#include "Debug/debug.h"

namespace Ming3D
{
    struct Vertex
    {
    public:
        glm::vec3 mPosition;
        glm::vec3 mNormal;
        glm::vec2 mTexCoords;
    };

    void CreateRotatedPlane(const std::vector<Vertex>& baseVerts, const std::vector<unsigned int>& baseIndices,
        const glm::vec3& rotDir, float rotAngle, const glm::vec3 posOffset,
        std::vector<Vertex>& outVerts, std::vector<unsigned int>& outIndices)
    {
        const size_t indexOffset = outVerts.size();

        for (size_t iVert = 0; iVert < baseVerts.size(); iVert++)
        {
            Vertex vertex = baseVerts[iVert];
            vertex.mPosition = glm::rotate(baseVerts[iVert].mPosition, rotAngle, rotDir) + posOffset;
            vertex.mNormal = glm::normalize(posOffset);
            outVerts.push_back(vertex);
        }

        for (size_t iIndex = 0; iIndex < baseIndices.size(); iIndex++)
        {
            outIndices.push_back(static_cast<unsigned int>(baseIndices[iIndex] + indexOffset));
        }
    }

    std::unique_ptr<Mesh> PrimitiveFactory::CreateBox(const glm::vec3& boxSize)
    {
        const float dx = boxSize.x / 2.0f;
        const float dy = boxSize.y / 2.0f;
        const float dz = boxSize.y / 2.0f;

        const std::vector<Vertex> baseVerts = {
            Vertex{ glm::vec3(-dx, -dy, 0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)  },
            Vertex{ glm::vec3(-dx, dy, 0),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)  },
            Vertex{ glm::vec3(dx, dy, 0),   glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)  },
            Vertex{ glm::vec3(dx, -dy, 0),  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)  },
        };

        const std::vector<unsigned int> baseIndices = { 0, 2, 1, 0, 3, 2 };

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        float deg90 = 3.141592654f / 2.0f;

        CreateRotatedPlane(baseVerts, baseIndices, glm::vec3(0, 1, 0), deg90 * 0.0f, glm::vec3(0, 0, dz), vertices, indices); // front
        CreateRotatedPlane(baseVerts, baseIndices, glm::vec3(0, 1, 0), deg90 * 1.0f, glm::vec3(dx, 0, 0), vertices, indices); // right
        CreateRotatedPlane(baseVerts, baseIndices, glm::vec3(0, 1, 0), deg90 * 2.0f, glm::vec3(0, 0, -dz), vertices, indices); // back
        CreateRotatedPlane(baseVerts, baseIndices, glm::vec3(0, 1, 0), deg90 * 3.0f, glm::vec3(-dx, 0, 0), vertices, indices); // left
        CreateRotatedPlane(baseVerts, baseIndices, glm::vec3(1, 0, 0), deg90 * 3.0f, glm::vec3(0, dy, 0), vertices, indices); // top
        CreateRotatedPlane(baseVerts, baseIndices, glm::vec3(1, 0, 0), deg90 * 1.0f, glm::vec3(0, -dy, 0), vertices, indices); // bottom

        std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
        // TODO: Add support for different vertex layouts (with/without normals and texcoords)
        mesh->mVertexData = new Rendering::VertexData({ Rendering::EVertexComponent::Position, Rendering::EVertexComponent::Normal, Rendering::EVertexComponent::TexCoord }, indices.size());
        mesh->mIndexData = new Rendering::IndexData(indices.size());

        memcpy(mesh->mVertexData->GetDataPtr(), vertices.data(), vertices.size() * sizeof(Vertex));
        memcpy(mesh->mIndexData->GetData(), indices.data(), indices.size() * sizeof(unsigned int));

        return mesh;
    }

    std::unique_ptr<Mesh> PrimitiveFactory::CreatePlane(const glm::vec2& planeSize, unsigned int dimX, unsigned int dimY)
    {
        if(dimX < 1 || dimY < 1)
        {
            LOG_ERROR() << "PrimitiveFactory::CreatePlane: Dimension must be at least 1";
            return nullptr;
        }

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for(unsigned int iy = 0; iy <= dimY; iy++)
        {
            float ty = iy / (float)dimY;
            for(unsigned int ix = 0; ix <= dimX; ix++)
            {
                float tx = ix / (float)dimX;
                Vertex vert;
                vert.mPosition = glm::vec3((tx  - 0.5f) * planeSize.x, 0.0f, (ty  - 0.5f) * planeSize.y);
                vert.mTexCoords = glm::vec2(tx, ty);
                vert.mNormal = glm::vec3(0.0f, 1.0f, 0.0f);
                vertices.push_back(vert);
            }
        }

        for(unsigned int iy = 1; iy <= dimY; iy++)
        {
            for(unsigned int ix = 1; ix <= dimX; ix++)
            {
                unsigned int i0 = (iy - 1) * (dimX + 1) + (ix - 1);
                unsigned int i1 = (iy - 1) * (dimX + 1) + (ix);
                unsigned int i2 = (iy) * (dimX + 1) + (ix - 1);
                unsigned int i3 = (iy) * (dimX + 1) + (ix);
                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i3);
                indices.push_back(i0);
                indices.push_back(i3);
                indices.push_back(i1);
            }
        }

        std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
        // TODO: Add support for different vertex layouts (with/without normals and texcoords)
        mesh->mVertexData = new Rendering::VertexData({ Rendering::EVertexComponent::Position, Rendering::EVertexComponent::Normal, Rendering::EVertexComponent::TexCoord }, vertices.size());
        mesh->mIndexData = new Rendering::IndexData(indices.size());

        memcpy(mesh->mVertexData->GetDataPtr(), vertices.data(), vertices.size() * sizeof(Vertex));
        memcpy(mesh->mIndexData->GetData(), indices.data(), indices.size() * sizeof(unsigned int));

        return mesh;
    }
}
