#ifndef MING3D_INDEX_BUFFER_H
#define MING3D_INDEX_BUFFER_H

namespace Ming3D
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
        void SetNumIndices(unsigned int inNumIndices);
        unsigned int GetNumIndices();
    };
}

#endif
