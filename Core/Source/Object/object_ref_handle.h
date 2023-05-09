#pragma once

#include <vector>

/*=============================================================================================
A reference handle that contains:
- raw pointer to a Ming3D::Object
- reference count

ObjectRefHandle should only belong to, and be instansiated by, a Ming3D::Object.
When the Ming3D::Object is destroyed, the object pointer of the handle will be sett to nullptr.
When the reference count is 0 and the object is destroyed, the ObjectRefHandle will be destroyed.

==============================================================================================*/
namespace Ming3D
{
    class Object;

    class ObjectRefHandle
    {
    private:
        /** The owning object */
        Object* mObject;

        /** Weak references (used by WeakObjectPtr) */
        unsigned int mRefCount;

        void DeleteIfAbandoned();

    public:
        explicit ObjectRefHandle(Object* object);

        /** Gets the owning object of the handle */
        inline Object* GetObject() const { return mObject; };

        /** Clears the owning object of the handle. */
        void ReleaseObject();

        /** Adds a reference to the object. */
        void AddRef();

        /** Removes a reference to the object. */
        void RemoveRef();
    };
}
