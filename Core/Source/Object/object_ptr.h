#pragma once

/*=============================================================================================
Weak pointer to a Ming3D::Object.
When the object is destroyed (in any way), this will return nullptr.

==============================================================================================*/
#include "object_ref_handle.h"
#include <cassert>
#include "object.h"

namespace Ming3D
{
    template <class T>
    class ObjectPtr
    {
    private:
        ObjectRefHandle* mRefHandle;

        Object* GetObjectSafe() const;

    public:
        ObjectPtr();
        ObjectPtr(Object* object);
        ObjectPtr(const ObjectPtr<T>& other);

        ~ObjectPtr();

        ObjectPtr<T>& operator=(const ObjectPtr<T>& other);
        ObjectPtr<T>& operator=(Object* other);

        T* operator->() const;
        bool operator==(const ObjectPtr<T>& arg_other) const;
        bool operator!=(const ObjectPtr<T>& arg_other) const;
        bool operator==(const T* arg_other) const;
        bool operator!=(const T* arg_other) const;

        T* Get() const;
        bool IsValid() const;
    };

    template <class T>
    Object* ObjectPtr<T>::GetObjectSafe() const
    {
        return (mRefHandle != nullptr ? mRefHandle->GetObject() : nullptr);
    }

    template <class T>
    ObjectPtr<T>::ObjectPtr()
    : mRefHandle(nullptr)
    {
        //static_assert((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");
    }

    template <class T>
    ObjectPtr<T>::ObjectPtr(Object* object)
    {
        //static_assert((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");

        if (object != nullptr)
        {
            mRefHandle = object->GetRefHandle();
            mRefHandle->AddRef();
        }
        else
        {
            mRefHandle = nullptr;
        }
    }

    template <class T>
    ObjectPtr<T>::ObjectPtr(const ObjectPtr<T>& other)
    {
        static_assert((std::is_base_of<Object, T>::value), "Type of T must be Object or subclass of Object.");

        if (other != nullptr)
        {
            mRefHandle = other.mRefHandle;
            mRefHandle->AddRef();
        }
        else
        {
            mRefHandle = nullptr;
        }
    }

    template <class T>
    ObjectPtr<T>::~ObjectPtr()
    {
        if (mRefHandle != nullptr)
            this->mRefHandle->RemoveRef();
    }

    template <class T>
    ObjectPtr<T>& ObjectPtr<T>::operator=(const ObjectPtr<T>& other)
    {
        ObjectRefHandle* oldHandle = this->mRefHandle;
        this->mRefHandle = other.mRefHandle;
        if (this->mRefHandle != nullptr)
            this->mRefHandle->AddRef();
        if (oldHandle != nullptr)
            oldHandle->RemoveRef();
        return (*this);
    }

    template <class T>
    ObjectPtr<T>& ObjectPtr<T>::operator=(Object* other)
    {
        ObjectRefHandle* oldHandle = this->mRefHandle;
        this->mRefHandle = other->GetRefHandle();
        if (this->mRefHandle != nullptr)
            this->mRefHandle->AddRef();
        if (oldHandle != nullptr)
            oldHandle->RemoveRef();
        return (*this);
    }

    template <class T>
    T* ObjectPtr<T>::operator->() const
    {
        return static_cast<T*>(this->mRefHandle->GetObject());
    }

    template <class T>
    bool ObjectPtr<T>::operator==(const ObjectPtr<T>& arg_other) const
    {
        return this->GetObjectSafe() == arg_other.GetObjectSafe();
    }

    template <class T>
    bool ObjectPtr<T>::operator!=(const ObjectPtr<T>& arg_other) const
    {
        return this->GetObjectSafe() != arg_other.GetObjectSafe();
    }

    template <class T>
    bool ObjectPtr<T>::operator==(const T* arg_other) const
    {
        return this->GetObjectSafe() == arg_other;
    }

    template <class T>
    bool ObjectPtr<T>::operator!=(const T* arg_other) const
    {
        return this->GetObjectSafe() != arg_other;
    }

    template <class T>
    T* ObjectPtr<T>::Get() const
    {
        return static_cast<T*>(this->GetObjectSafe());
    }

    template <class T>
    bool ObjectPtr<T>::IsValid() const
    {
        return this->GetObjectSafe() != nullptr;
    }

    class Actor;

    using ActorPtr = ObjectPtr<Actor>;
}
