#include "d2ePch.h"
#include "Movement.h"

#include "RigidBody.h"
#include "CircleCollider.h"

#include "ES/GameObject.h"

namespace d2e
{

void Movement::Update(const float dt)
{
    WeakRef<RigidBody> rigidBody = mParent->GetComponent<d2e::RigidBody>();
    const WeakRef<CircleCollider> circleCollider = mParent->GetComponent<CircleCollider>();

    const bool jumped = currentFrameJumpCount > previousFrameJumpCount;

    // If we collided last frame, negate and then multiply the count means we multiply by 0 and hence reset the count.
    // Else we multiply by 1 and hence have the same jump count.
    currentFrameJumpCount = static_cast<uint16_t>(!circleCollider->GetCollidedLastFrame()) * currentFrameJumpCount;

    if (jumped && currentFrameJumpCount <= 2)
    {
        rigidBody->AddVelocity(Vec2{ 0.0f, 8.0f });
    }

    rigidBody->AddForce(Vec2{ xAxisDelta * speed, 0.0f });

    previousFrameJumpCount = currentFrameJumpCount;
}

std::string Movement::Serialize() const
{
    return SerializeUtils::Serialize(mSyncValuesOnUpdate)
         + SerializeUtils::Serialize(xAxisDelta)            + SerializeUtils::DELIMITER
         + SerializeUtils::Serialize(speed)                 + SerializeUtils::DELIMITER
         + SerializeUtils::Serialize(maxJumpCount)          + SerializeUtils::DELIMITER
         + SerializeUtils::Serialize(currentFrameJumpCount) + SerializeUtils::DELIMITER
         + SerializeUtils::Serialize(previousFrameJumpCount);
}

void Movement::Deserialize(const std::string& string)
{
    SerializeUtils::Deserialize(mSyncValuesOnUpdate, std::string{ string[0] });

    const size_t firstDelimiter   = string.find(SerializeUtils::DELIMITER);
    const size_t secondDelimiter  = string.find(SerializeUtils::DELIMITER, firstDelimiter  + 1);
    const size_t thirdDelimiter   = string.find(SerializeUtils::DELIMITER, secondDelimiter + 1);
    const size_t fourthDelimiter  = string.find(SerializeUtils::DELIMITER, thirdDelimiter  + 1);

    const std::string first  = string.substr(1, firstDelimiter);
    const std::string second = string.substr(firstDelimiter  + 1, secondDelimiter - firstDelimiter  - 1);
    const std::string third  = string.substr(secondDelimiter + 1, thirdDelimiter  - secondDelimiter - 1);
    const std::string fourth = string.substr(thirdDelimiter  + 1, fourthDelimiter - thirdDelimiter  - 1);
    const std::string fifth  = string.substr(fourthDelimiter + 1);

    SerializeUtils::Deserialize(xAxisDelta, first);
    SerializeUtils::Deserialize(speed, second);
    SerializeUtils::Deserialize(maxJumpCount, third);
    SerializeUtils::Deserialize(currentFrameJumpCount, fourth);
    SerializeUtils::Deserialize(previousFrameJumpCount, fifth);
}
} // Namespace d2e.
