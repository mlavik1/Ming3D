#ifndef MING3D_RENDERSCENE_H
#define MING3D_RENDERSCENE_H

#include "render_object.h"
#include <vector>

namespace Ming3D
{
    class RenderScene
    {
    public:
        std::vector<RenderObject*> mSceneObjects;

    };
}

#endif
