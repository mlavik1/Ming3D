#pragma once
#include "SceneRenderer/render_type.h"
#include "glm/vec3.hpp"
#include <cstddef>

namespace Ming3D
{
    class RenderBatch;

    class RenderObject
    {
    public:
        virtual ~RenderObject() {}

        /* Gets the number of render batches in this object. */
        virtual size_t GetNumBatches() = 0;
        /* Gets the specified render batch (at the specified index). */
        virtual void GetRenderBatch(size_t batchIndex, RenderBatch* outBatch) = 0;

        virtual ERenderType GetRenderType() = 0;

        virtual bool GetCastShadows() = 0;

        virtual glm::vec3 GetWorldPosition() = 0;
    };
}
