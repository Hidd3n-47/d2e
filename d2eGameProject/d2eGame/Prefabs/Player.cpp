#include "Player.h"

#include <d2e/Es/Components/Movement.h>
#include <d2e/ES/Components/RigidBody.h>
#include <d2e/ES/Components/CircleSprite.h>
#include <d2e/ES/Components/CircleCollider.h>

namespace d2eGame
{

void Player::CreatePrefab(d2e::WeakRef<d2e::Scene> scene)
{
    mGameObject = scene->CreateGameObject();

    constexpr float PLAYER_RADIUS = 20.0f;

    mGameObject->GetComponent<d2e::Transform>()->translation = d2e::Vec2{ 1000.0f, 100.0f };

    auto visual = mGameObject->AddComponent<d2e::CircleSprite>();
    visual->SetColor(sf::Color::Yellow);
    visual->SetRadius(PLAYER_RADIUS);

    auto collider = mGameObject->AddComponent<d2e::CircleCollider>();
    collider->SetRadius(PLAYER_RADIUS);

    auto movement = mGameObject->AddComponent<d2e::Movement>();
    movement->SetSpeed(3.0f);

    auto rb = mGameObject->AddComponent<d2e::RigidBody>();
    rb->SetGravity(d2e::Vec2{ 0.0f, 0.2f });
    rb->SetRestitution(0.25f);
}

} // Namespace d2eGame.