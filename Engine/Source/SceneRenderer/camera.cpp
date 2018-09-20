#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Ming3D
{
    Camera::Camera()
    {
        mCameraMatrix = glm::lookAt(
            glm::vec3(0, 2, 6), // pos
            glm::vec3(0, 0, 0), // lookat
            glm::vec3(0, 1, 0)  // up
        );
    }
}
