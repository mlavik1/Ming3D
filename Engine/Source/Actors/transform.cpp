#include "transform.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
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
        mWorldPosition = mParentTransform == nullptr ? mLocalPosition : mParentTransform->GetWorldTransformMatrix() * glm::vec4(mLocalPosition, 1.0f);
        UpdateTransformMatrix();
    }

    void Transform::SetLocalScale(glm::vec3 inScale)
    {
        mLocalScale = inScale;
        mWorldScale = mParentTransform == nullptr ? mLocalScale : mParentTransform->GetWorldTransformMatrix() * glm::vec4(mLocalScale, 1.0f);
        UpdateTransformMatrix();
    }

    void Transform::SetLocalRotation(glm::quat inRot)
    {
        mLocalRotation = inRot;
        mWorldRotation = mParentTransform == nullptr ? mLocalRotation : mParentTransform->GetWorldTransformMatrix() * glm::toMat4(mLocalRotation);
        UpdateTransformMatrix();
    }

    void Transform::SetLocalRotation(glm::mat4 inRot)
    {
        mLocalRotation = inRot;
        mWorldRotation = mParentTransform == nullptr ? mLocalRotation : mParentTransform->GetWorldTransformMatrix() * glm::toMat4(mLocalRotation);
        UpdateTransformMatrix();
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

    void Transform::UpdateTransformMatrix()
    {
        mLocalTransformMatrix = glm::translate(glm::mat4(1.0f), mWorldPosition) * glm::toMat4(mWorldRotation) * glm::scale(glm::mat4(1.0f), mWorldScale);
        mWorldTransformMatrix = mParentTransform == nullptr ? mLocalTransformMatrix : mParentTransform->GetWorldTransformMatrix() * GetLocalTransformMatrix();

        if(mActor != nullptr)
            mActor->OnTransformMoved();
    }
}
