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
    WeakRef<RigidBody> rigidBody = mParent->GetComponent<RigidBody>();
    const WeakRef<CircleCollider> circleCollider = mParent->GetComponent<CircleCollider>();

    // If we collided last frame, negate and then multiply the count means we multiply by 0 and hence reset the count.
    // Else we multiply by 1 and hence have the same jump count.
    mJumpCount = static_cast<uint16_t>(!circleCollider->GetCollidedLastFrame()) * mJumpCount;

    float xAxisDelta = 0.0f;

    const WeakRef<InputManager> inputManager = Engine::Instance()->GetInputManager();
    if (inputManager->IsKeyDown(sf::Keyboard::Key::A))
    {
        xAxisDelta -= 1.0f;
    }
    if (inputManager->IsKeyDown(sf::Keyboard::Key::D))
    {
        xAxisDelta += 1.0f;
    }
    if (inputManager->IsKeyPressed(sf::Keyboard::Key::Space) && mJumpCount < mMaxJumpCount)
    {
        ++mJumpCount;
        rigidBody->AddVelocity(Vec2{ 0.0f, -7.5f });
    }

    const float desiredSpeed = xAxisDelta * (mSpeed * 0.02f);

    rigidBody->AddForce(Vec2{ desiredSpeed, 0.0f });
}

} // Namespace d2e.
