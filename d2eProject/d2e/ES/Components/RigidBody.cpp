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
    //float mMass = 1.0f;
    //float mRestitution = 0.1f;
    //float mMaxSpeed = 500.0f;
    //Vec2 mVelocity;
    //Vec2 mAcceleration;
    //Vec2 mGravity;
    //Vec2 mForce;
    const std::string delimiter = "|";

    return SerializeUtils::Serialize(mMass) +
            delimiter + SerializeUtils::Serialize(mRestitution) +
            delimiter + SerializeUtils::Serialize(mMaxSpeed) +
            delimiter + SerializeUtils::Serialize(mVelocity) +
            delimiter + SerializeUtils::Serialize(mAcceleration) +
            delimiter + SerializeUtils::Serialize(mGravity) +
            delimiter + SerializeUtils::Serialize(mForce);

}

void RigidBody::Deserialize(const std::string& string)
{
    const std::string delimiter = "|";

    const size_t firstDelimiter  = string.find(delimiter);
    const size_t secondDelimiter = string.find(delimiter, firstDelimiter  + 1);
    const size_t thirdDelimiter  = string.find(delimiter, secondDelimiter + 1);
    const size_t fourthDelimiter = string.find(delimiter, thirdDelimiter  + 1);
    const size_t fifthDelimiter  = string.find(delimiter, fourthDelimiter + 1);
    const size_t sixthDelimiter  = string.find(delimiter, fifthDelimiter  + 1);

    const std::string first   = string.substr(0, firstDelimiter);
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
