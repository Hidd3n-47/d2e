#include "d2ePch.h"
#include "Movement.h"

#include "Core/Engine.h"
#include "ES/GameObject.h"
#include "Input/InputManager.h"

namespace d2e
{

void Movement::OnComponentAdded(const WeakRef<GameObject> parent)
{
    IComponent::OnComponentAdded(parent);
}

void Movement::Update(const float dt)
{
    WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    Vec2 movement;
    WeakRef<InputManager> inputManager = Engine::Instance()->GetInputManager();

    if (inputManager->IsKeyDown(sf::Keyboard::Key::W))
    {
        movement += Vec2{ 0.0f, -1.0f };
    }
    if (inputManager->IsKeyDown(sf::Keyboard::Key::S))
    {
        movement += Vec2{ 0.0f, 1.0f };
    }
    if (inputManager->IsKeyDown(sf::Keyboard::Key::A))
    {
        movement += Vec2{ -1.0f, 0.0f };
    }
    if (inputManager->IsKeyDown(sf::Keyboard::Key::D))
    {
        movement += Vec2{ 1.0f, 0.0f };
    }

    movement.Normalize();

    transform->translation += movement * mSpeed;
}

} // Namespace d2e.
