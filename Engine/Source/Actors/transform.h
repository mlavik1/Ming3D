#ifndef MING3D_TRANSFORM_H
#define MING3D_TRANSFORM_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <list>

namespace Ming3D
{
    class Actor;

    class Transform
    {
        friend class Actor;

    private:
        glm::vec3 mLocalPosition;
        glm::vec3 mLocalScale;
        glm::quat mLocalRotation;

        glm::vec3 mWorldPosition;
        glm::vec3 mWorldScale;
        glm::quat mWorldRotation;

        Actor* mActor = nullptr;
        Transform* mParentTransform = nullptr;
        std::list<Transform*> mChildren;

        glm::mat4 mLocalTransformMatrix;
        glm::mat4 mWorldTransformMatrix;

        void UpdateTransformMatrix();

    public:
        Transform();

        void SetLocalPosition(glm::vec3 inPosition);
        void SetLocalScale(glm::vec3 inScale);
        void SetLocalRotation(glm::quat inRot);
        void SetLocalRotation(glm::mat4 inRot);
        void SetLocalTransformMatrix(glm::mat4 mat);

        void SetWorldPosition(glm::vec3 inPosition);
        void SetWorldScale(glm::vec3 inScale);
        void SetWorldRotation(glm::quat inRot);
        void SetWorldRotation(glm::mat4 inRot);

        void SetParent(Transform* inParent);

        void Rotate(float inAngle, const glm::vec3& inAxis);

        glm::vec3 GetForward() const;
        glm::vec3 GetUp() const;
        glm::vec3 GetRight() const;

        inline glm::vec3 GetLocalPosition() { return mLocalPosition; }
        inline glm::vec3 GetLocalScale() { return mLocalScale; }
        inline glm::quat GetLocalRotation() { return mLocalRotation; }

        glm::vec3 GetWorldPosition() const { return mWorldPosition; }
        glm::vec3 GetWorldScale() const { return mWorldScale; }
        glm::quat GetWorldRotation() const { return mWorldRotation; }

        inline glm::mat4 GetLocalTransformMatrix() { return mLocalTransformMatrix; }
        inline glm::mat4 GetWorldTransformMatrix() { return mWorldTransformMatrix; }
    };
}

#endif
