#ifndef MING3D_GAMEOBJECT_H
#define MING3D_GAMEOBJECT_H

#include "object.h"
#include "objdefs.h"

namespace Ming3D
{
    class GameObject : public Object
    {
        DEFINE_CLASS(Ming3D::GameObject, Ming3D::Object)

    private:
        static void InitialiseClass();

    public:
        netguid_t mNetGUID = 0;

    };
}

#endif
