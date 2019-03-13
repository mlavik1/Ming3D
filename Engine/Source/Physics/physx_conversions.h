#ifndef MING3D_PHYSXCONVERSIONS_H
#define MING3D_PHYSXCONVERSIONS_H

#include "foundation/PxVec3.h"
#include "foundation/PxQuat.h"
#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"

namespace Ming3D
{
    namespace PhysXConversions
    {
        inline physx::PxVec3 glmVec3ToPxVec3(const glm::vec3& vec)
        {
            return physx::PxVec3(vec.x, vec.y, vec.z);
        }

        inline physx::PxQuat glmQuatToPxQuat(const glm::quat& quat)
        {
            return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
        }

        inline glm::vec3 pxVec3ToGlmVec3(const physx::PxVec3& vec)
        {
            return glm::vec3(vec.x, vec.y, vec.z);
        }

        inline glm::quat pxQuatToGlmQuat(const physx::PxQuat& quat)
        {
            return glm::quat(quat.w, quat.x, quat.y, quat.z);
        }
    };
}

#endif
