#include "d2ePch.h"
#include "Movement.h"

#include "RigidBody.h"
#include "CircleCollider.h"

#include "Core/Engine.h"
#include "ES/GameObject.h"
#include "Input/InputManager.h"

namespace d2e
{

void Movement::Update(const float dt)
{
    WeakRef<RigidBody> rigidBody = mParent->GetComponent<d2e::RigidBody>();
    const WeakRef<CircleCollider> circleCollider = mParent->GetComponent<CircleCollider>();

    // If we collided last frame, negate and then multiply the count means we multiply by 0 and hence reset the count.
    // Else we multiply by 1 and hence have the same jump count.
    jumpCount = static_cast<uint16_t>(!circleCollider->GetCollidedLastFrame()) * jumpCount;

    if (jumped && jumpCount < 2)
    {
        ++jumpCount;
        rigidBody->AddVelocity(Vec2{ 0.0f, 8.0f });
    }

    const float desiredSpeed = xAxisDelta * speed;

    rigidBody->AddForce(Vec2{ desiredSpeed, 0.0f });
}

std::string Movement::Serialize() const
{
    return SerializeUtils::Serialize(mSyncValuesOnUpdate)
         + SerializeUtils::Serialize(jumped)
         + SerializeUtils::Serialize(xAxisDelta)   + SerializeUtils::DELIMITER
         + SerializeUtils::Serialize(speed)        + SerializeUtils::DELIMITER
         + SerializeUtils::Serialize(maxJumpCount) + SerializeUtils::DELIMITER
         + SerializeUtils::Serialize(jumpCount);
}

void Movement::Deserialize(const std::string& string)
{
    SerializeUtils::Deserialize(mSyncValuesOnUpdate, std::string{ string[0] });
    SerializeUtils::Deserialize(jumped, std::string{ string[1] });

    const size_t firstDelimiter  = string.find(SerializeUtils::DELIMITER);
    const size_t secondDelimiter = string.find(',', firstDelimiter + 1);
    const size_t thirdDelimiter  = string.find(',', secondDelimiter + 1);

    const std::string first  = string.substr(2, firstDelimiter);
    const std::string second = string.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1);
    const std::string third  = string.substr(secondDelimiter + 1, thirdDelimiter - secondDelimiter - 1);
    const std::string fourth = string.substr(thirdDelimiter + 1);

    SerializeUtils::Deserialize(xAxisDelta, first);
    SerializeUtils::Deserialize(speed, second);
    SerializeUtils::Deserialize(maxJumpCount, third);
    SerializeUtils::Deserialize(jumpCount, fourth);
}
} // Namespace d2e.
