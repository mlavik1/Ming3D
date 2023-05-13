#include "transform.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "actor.h"

namespace Ming3D
{
    Transform::Transform()
    {
        SetLocalScale(glm::vec3(1.0f));
    }

    void Transform::SetLocalPosition(glm::vec3 inPosition)
    {
        mLocalPosition = inPosition;
        UpdateTransformMatrix();
    }

    void Transform::SetLocalScale(glm::vec3 inScale)
    {
        mLocalScale = inScale;
        UpdateTransformMatrix();
    }

    void Transform::SetLocalRotation(glm::quat inRot)
    {
        mLocalRotation = inRot;
        UpdateTransformMatrix();
    }

    void Transform::SetLocalRotation(glm::mat4 inRot)
    {
        mLocalRotation = inRot;
        UpdateTransformMatrix();
    }

    void Transform::SetLocalTransformMatrix(glm::mat4 mat)
    {
        // TODO: Optimise this
        
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(mat, scale, rotation, translation, skew, perspective);
        SetLocalPosition(translation);
        SetLocalScale(scale);
        SetLocalRotation(rotation);
    }

    void Transform::SetWorldPosition(glm::vec3 inPosition)
    {
        mWorldPosition = inPosition;
        mLocalPosition = mParentTransform == nullptr ? mWorldPosition : glm::inverse(mParentTransform->GetWorldTransformMatrix()) * glm::vec4(mWorldPosition, 1.0f);
        UpdateTransformMatrix();
    }

    void Transform::SetWorldScale(glm::vec3 inScale)
    {
        mWorldScale = inScale;
        mLocalScale = mParentTransform == nullptr ? mWorldScale : glm::inverse(mParentTransform->GetWorldTransformMatrix()) * glm::vec4(mWorldScale, 1.0f);
        UpdateTransformMatrix();
    }

    void Transform::SetWorldRotation(glm::quat inRot)
    {
        mWorldRotation = inRot;
        mLocalRotation = mParentTransform == nullptr ? mWorldRotation : glm::inverse(mParentTransform->GetWorldTransformMatrix()) * glm::toMat4(mWorldRotation);
        UpdateTransformMatrix();
    }

    void Transform::SetWorldRotation(glm::mat4 inRot)
    {
        mWorldRotation = inRot;
        mLocalRotation = mParentTransform == nullptr ? mWorldRotation : glm::inverse(mParentTransform->GetWorldTransformMatrix()) * glm::toMat4(mWorldRotation);
        UpdateTransformMatrix();
    }

    void Transform::Rotate(float inAngle, const glm::vec3& inAxis)
    {
        glm::quat newRot = glm::rotate(GetWorldRotation(), inAngle, inAxis);
        SetWorldRotation(newRot);
    }

    glm::vec3 Transform::GetForward() const
    {
        return glm::normalize(mWorldTransformMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    }

    glm::vec3 Transform::GetUp() const
    {
        return glm::normalize(mWorldTransformMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    }

    glm::vec3 Transform::GetRight() const
    {
        return glm::normalize(mWorldTransformMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    }

    void Transform::UpdateTransformMatrix()
    {
        mWorldPosition = mParentTransform == nullptr ? mLocalPosition : mParentTransform->GetWorldTransformMatrix() * glm::vec4(mLocalPosition, 1.0f);
        mWorldScale = mParentTransform == nullptr ? mLocalScale : mParentTransform->GetWorldTransformMatrix() * glm::vec4(mLocalScale, 1.0f);
        mWorldRotation = mParentTransform == nullptr ? mLocalRotation : mParentTransform->GetWorldTransformMatrix() * glm::toMat4(mLocalRotation);

        mLocalTransformMatrix = glm::translate(glm::mat4(1.0f), mLocalPosition) * glm::toMat4(mLocalRotation) * glm::scale(glm::mat4(1.0f), mLocalScale);
        mWorldTransformMatrix = mParentTransform == nullptr ? mLocalTransformMatrix : mParentTransform->GetWorldTransformMatrix() * GetLocalTransformMatrix();

        if (mOnTransformMoved)
            mOnTransformMoved();

        for (Transform* child : mChildren) // TODO
            child->UpdateTransformMatrix();
    }
}
