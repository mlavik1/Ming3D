#ifndef MING3D_RENDERSCENE_H
#define MING3D_RENDERSCENE_H

#include "render_scene_object.h"
#include <vector>

namespace Ming3D
{
    class RenderScene
    {
    public:
        std::vector<RenderSceneObject*> mSceneObjects;

    };
}

#endif
