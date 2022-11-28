#pragma once
#include "SceneRenderer/render_type.h"
#include "glm/vec3.hpp"

namespace Ming3D
{
    class RenderBatch;

    class RenderObject
    {
    public:
        virtual ~RenderObject() {}

        /* Gets the number of render batches in this object. */
        virtual int GetNumBatches() = 0;
        /* Gets the specified render batch (at the specified index). */
        virtual void GetRenderBatch(int batchIndex, RenderBatch* outBatch) = 0;

        virtual ERenderType GetRenderType() = 0;

        virtual bool GetCastShadows() = 0;

        virtual glm::vec3 GetWorldPosition() = 0;
    };
}
