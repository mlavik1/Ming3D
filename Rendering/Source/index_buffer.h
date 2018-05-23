#ifndef MING3D_INDEX_BUFFER_H
#define MING3D_INDEX_BUFFER_H

namespace Ming3D
{
    class IndexBuffer
    {
    private:
        unsigned int mNumIndices;

    public:
        void SetNumIndices(unsigned int inNumIndices);
        unsigned int GetNumIndices();
    };
}

#endif
