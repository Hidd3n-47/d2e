#include "d2ePch.h"
#include "Movement.h"

#include "RigidBody.h"
#include "Core/Engine.h"
#include "ES/GameObject.h"
#include "Input/InputManager.h"

namespace d2e
{

void Movement::Update(const float dt)
{
    WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    float xAxisDelta = 0.0f;
    Vec2 movement;
    WeakRef<InputManager> inputManager = Engine::Instance()->GetInputManager();

    if (inputManager->IsKeyDown(sf::Keyboard::Key::A))
    {
        movement += Vec2{ -1.0f, 0.0f };
        xAxisDelta -= 1.0f;
    }
    if (inputManager->IsKeyDown(sf::Keyboard::Key::D))
    {
        movement += Vec2{ 1.0f, 0.0f };
        xAxisDelta += 1.0f;
    }
    if (inputManager->IsKeyPressed(sf::Keyboard::Key::Space))
    {
        mParent->GetComponent<RigidBody>()->AddVelocity(Vec2{ 0.0f, -7.5f });
    }

    //movement.Normalize();

    // inputDir = -1, 0, +1
    float desiredSpeed = xAxisDelta * (mSpeed * 0.02f);
    float velocityDelta = std::min(desiredSpeed, mParent->GetComponent<RigidBody>()->GetVelocity().x);

    // accelerationFactor controls responsiveness
    constexpr float accelerationFactor = 0.2f;
    float forceX = velocityDelta * accelerationFactor;

    //transform->translation += movement * mSpeed;
    //mParent->GetComponent<RigidBody>()->AddForce(movement * mSpeed / 100.0f);
    mParent->GetComponent<RigidBody>()->AddForce(Vec2{ desiredSpeed, 0.0f });

    //if (abs(mParent->GetComponent<RigidBody>()->GetVelocity().x) < 0.01f)
    //    mParent->GetComponent<RigidBody>()->Set();

}

} // Namespace d2e.
