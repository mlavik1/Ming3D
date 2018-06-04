#ifndef MING3D_GRAPHICS_DATA_H
#define MING3D_GRAPHICS_DATA_H

#include <vector>

#include <glm/glm.hpp>

namespace Ming3D
{
    enum EVertexComponent
    {
        Position,
        Normal,
        TexCoord,
        Colour
    };

    /**
    * Vertex Layout.
    * Contains information about vertices (type, size).
    */
    struct VertexLayout
    {
        std::vector<EVertexComponent> VertexComponents;
    };

    namespace VertexDataType
    {
        struct Vertex
        {
            glm::vec3 Position;
        };
        struct VertexNormal
        {
            glm::vec3 Position;
            glm::vec3 Normal;
        };
        struct VertexNormalTexCoord
        {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 TexCoord;
        };
    }

    /**
    * Vertex Data.
    * Contains a set of vertices, and their vertex layout.
    */
    class VertexData
    {
    private:
        std::vector<char> mData;
        VertexLayout mVertexLayout;;
        size_t mVertexSize = 0;
        
    public:
        VertexData(std::vector<EVertexComponent> inComponents, size_t inNumVertices);
        void GetComponentOffsets(EVertexComponent inComponent, std::vector<size_t>& outOffsets);
        size_t GetNumVertices();
        size_t GetVertexSize();

        template<typename T>
        T* GetDataAs()
        {
            return (T*)mData.data();
        }

        void* GetDataPtr() { return mData.data(); }
        
        const VertexLayout& GetVertexLayout() { return mVertexLayout; }

        static size_t GetVertexComponentSize(EVertexComponent inComp);
    };

    /**
    * Index Data.
    * Contains a set of indices.
    */
    class IndexData
    {
    private:
        std::vector<unsigned int> mData;

    public:
        IndexData(size_t inNumIndices);
        size_t GetNumIndices();
        unsigned int* GetData() { return mData.data(); }
    };
}

#endif
