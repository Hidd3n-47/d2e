#include "d2ePch.h"
#include "RigidBody.h"

#include "ES/Scene.h"
#include "Core/SerializationUtils.h"

namespace d2e
{

void RigidBody::OnComponentAdded(const WeakRef<GameObject> parent)
{
    parent->GetScene()->GetPhysicsManager()->AddBody(parent);

    IComponent::OnComponentAdded(parent);
}

void RigidBody::OnComponentRemoved()
{
    mParent->GetScene()->GetPhysicsManager()->RemoveBody(mParent);
}

std::string RigidBody::Serialize() const
{
    return SerializeUtils::Serialize(mSyncValuesOnUpdate) + SerializeUtils::Serialize(mMass) +
           SerializeUtils::DELIMITER + SerializeUtils::Serialize(mRestitution) +
           SerializeUtils::DELIMITER + SerializeUtils::Serialize(mMaxSpeed) +
           SerializeUtils::DELIMITER + SerializeUtils::Serialize(mVelocity) +
           SerializeUtils::DELIMITER + SerializeUtils::Serialize(mAcceleration) +
           SerializeUtils::DELIMITER + SerializeUtils::Serialize(mGravity) +
           SerializeUtils::DELIMITER + SerializeUtils::Serialize(mForce);

}

void RigidBody::Deserialize(const std::string& string)
{
    SerializeUtils::Deserialize(mSyncValuesOnUpdate, std::string{ string[0] });

    const size_t firstDelimiter  = string.find(SerializeUtils::DELIMITER);
    const size_t secondDelimiter = string.find(SerializeUtils::DELIMITER, firstDelimiter  + 1);
    const size_t thirdDelimiter  = string.find(SerializeUtils::DELIMITER, secondDelimiter + 1);
    const size_t fourthDelimiter = string.find(SerializeUtils::DELIMITER, thirdDelimiter  + 1);
    const size_t fifthDelimiter  = string.find(SerializeUtils::DELIMITER, fourthDelimiter + 1);
    const size_t sixthDelimiter  = string.find(SerializeUtils::DELIMITER, fifthDelimiter  + 1);

    const std::string first   = string.substr(1, firstDelimiter);
    const std::string second  = string.substr(firstDelimiter  + 1, secondDelimiter - firstDelimiter - 1);
    const std::string third   = string.substr(secondDelimiter + 1, thirdDelimiter - secondDelimiter - 1);
    const std::string fourth  = string.substr(thirdDelimiter  + 1, fourthDelimiter - thirdDelimiter - 1);
    const std::string fifth   = string.substr(fourthDelimiter + 1, fifthDelimiter - fourthDelimiter - 1);
    const std::string sixth   = string.substr(fifthDelimiter  + 1, sixthDelimiter - fifthDelimiter - 1);
    const std::string seventh = string.substr(sixthDelimiter  + 1);

    SerializeUtils::Deserialize(mMass, first);
    SerializeUtils::Deserialize(mRestitution, second);
    SerializeUtils::Deserialize(mMaxSpeed, third);
    SerializeUtils::Deserialize(mVelocity, fourth);
    SerializeUtils::Deserialize(mAcceleration, fifth);
    SerializeUtils::Deserialize(mGravity, sixth);
    SerializeUtils::Deserialize(mForce, seventh);
}

void RigidBody::EnsureVelocityIsUnderMaxSpeed()
{
    if (const float len = mVelocity.Magnitude(); len > mMaxSpeed)
    {
        mVelocity /= len;
        mVelocity *= mMaxSpeed;
    }
}
} // Namespace d2e.
