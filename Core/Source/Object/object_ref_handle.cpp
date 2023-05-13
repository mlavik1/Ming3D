#include "object_ref_handle.h"
#include "object.h"

namespace Ming3D
{
    ObjectRefHandle::ObjectRefHandle(Object* object)
        : mObject(object), mRefCount(0)
    {
        DeleteIfAbandoned();
    }

    void ObjectRefHandle::ReleaseObject()
    {
        mObject = nullptr;
        DeleteIfAbandoned();
    }

    void ObjectRefHandle::AddRef()
    {
        mRefCount++;
    }

    void ObjectRefHandle::RemoveRef()
    {
        mRefCount--;

        DeleteIfAbandoned();
    }

    void ObjectRefHandle::DeleteIfAbandoned()
    {
        if (mObject == nullptr && mRefCount == 0)
        {
            delete this;
        }
    }
}
