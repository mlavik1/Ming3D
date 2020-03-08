#ifndef MING3D_INDEX_BUFFER_H
#define MING3D_INDEX_BUFFER_H

namespace Ming3D::Rendering
{
    /**
    * Index buffer base class.
    * There will be one sub-class for each rendering API.
    * Created by the RenderDevice.
    */
    class IndexBuffer
    {
    private:
        unsigned int mNumIndices;

    public:
        virtual ~IndexBuffer() = default;
        void SetNumIndices(unsigned int inNumIndices);
        unsigned int GetNumIndices();
    };
}

#endif
