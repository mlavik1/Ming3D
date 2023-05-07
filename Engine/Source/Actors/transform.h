#ifndef MING3D_TRANSFORM_H
#define MING3D_TRANSFORM_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <list>
#include <memory>
#include <functional>

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

        Transform* mParentTransform = nullptr;
        std::list<Transform*> mChildren;

        glm::mat4 mLocalTransformMatrix;
        glm::mat4 mWorldTransformMatrix;

        std::function<void(void)> mOnTransformMoved; // TODO: Add function for subscribing (multiple subscribers?)

        void UpdateTransformMatrix();

    public:
        Transform();

        // Delete copy constructor and assignment operator
        Transform(const Transform&) = delete;
        Transform& operator=(const Transform&) = delete;

        void SetLocalPosition(glm::vec3 inPosition);
        void SetLocalScale(glm::vec3 inScale);
        void SetLocalRotation(glm::quat inRot);
        void SetLocalRotation(glm::mat4 inRot);
        void SetLocalTransformMatrix(glm::mat4 mat);

        void SetWorldPosition(glm::vec3 inPosition);
        void SetWorldScale(glm::vec3 inScale);
        void SetWorldRotation(glm::quat inRot);
        void SetWorldRotation(glm::mat4 inRot);

        void Rotate(float inAngle, const glm::vec3& inAxis);

        Transform* GetParent() const { return mParentTransform; }
        
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
