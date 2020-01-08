#ifndef MING3D_CONSTANTBUFFERDATA_H
#define MING3D_CONSTANTBUFFERDATA_H

#include <vector>
#include <string>
#include <cstdio>
#include <glm/glm.hpp>
#include <type_traits>

namespace Ming3D
{
    class ConstantBufferDataBase
    {
public:
        virtual ~ConstantBufferDataBase() {}

        void* mDataPtr;
        size_t mSize;
    };

    template <typename... Types>
    class ConstantBufferDataGL : public ConstantBufferDataBase
    {
    private:
        size_t mPos = 0;

    public:
        std::vector<char> mData; // TODO: calculate size at compile time, and use array of fixed size

        ConstantBufferDataGL() {}
        virtual ~ConstantBufferDataGL() {}

    private:
        template <typename T, std::enable_if_t<std::is_same<T, float>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(float);
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

        template <typename T, std::enable_if_t<std::is_same<T, glm::vec2>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(glm::vec2);
            mPos += (8 - (mPos % 8)) % 8;
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

        template <typename T, std::enable_if_t<std::is_same<T, glm::vec3>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(glm::vec3);
            mPos += (16 - (mPos % 16)) % 16;
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

        template <typename T, std::enable_if_t<std::is_same<T, glm::vec4>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(glm::vec4);
            mPos += (16 - (mPos % 16)) % 16;
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

        template <typename T, std::enable_if_t<std::is_same<T, glm::mat4>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(glm::mat4);
            mPos += (16 - (mPos % 16) % 16);
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

    public:
        void SetData(Types... elements)
        {
            mPos = 0;
#if __cplusplus >= 201703L
            (SetElement(elements), ...);
#else
            std::initializer_list<int>{((void)SetElement(std::forward<Types>(elements)), 0)...};
#endif
            mDataPtr = &mData[0];
            mSize = mPos;
        }
    };

    template <typename... Types>
    class ConstantBufferDataD3D11 : public ConstantBufferDataBase
    {
    private:
        size_t mPos = 0;

    public:
        std::vector<char> mData; // TODO: calculate size at compile time, and use array of fixed size

        ConstantBufferDataD3D11() {}
        virtual ~ConstantBufferDataD3D11() {}

    private:
        template <typename T, std::enable_if_t<std::is_same<T, float>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(float);
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

        template <typename T, std::enable_if_t<std::is_same<T, glm::vec2>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(glm::vec2);
            mPos += (16 - (mPos % 16)) % 16;
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

        template <typename T, std::enable_if_t<std::is_same<T, glm::vec3>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(glm::vec3);
            mPos += (16 - (mPos % 16)) % 16;
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

        template <typename T, std::enable_if_t<std::is_same<T, glm::vec4>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(glm::vec4);
            mPos += (16 - (mPos % 16)) % 16;
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

        template <typename T, std::enable_if_t<std::is_same<T, glm::mat4>::value, int> = 0>
        void SetElement(T elem)
        {
            const size_t size = sizeof(glm::mat4);
            mPos += (16 - (mPos % 16) % 16);
            mData.resize(mPos + size);
            memcpy(&mData[mPos], &elem, size);
            mPos += size;
        }

    public:
        void SetData(Types... elements)
        {
            mPos = 0;
#if __cplusplus >= 201703L
            (SetElement(elements), ...);
#else
            std::initializer_list<int>{((void)SetElement(std::forward<Types>(elements)), 0)...};
#endif

            // Add final padding (must be multiple of 16)
            mPos += (16 - (mPos % 16) % 16);
            mData.resize(mPos);

            mDataPtr = &mData[0];
            mSize = mPos;
        }
};

    template <typename... Types>
    class ConstantBufferData
#ifdef MING3D_OPENGL
        : public ConstantBufferDataGL<Types...>
#else
        : public ConstantBufferDataD3D11<Types...>
#endif
    {
    };
}

#endif // MING3D_CONSTANTBUFFERDATA_H
