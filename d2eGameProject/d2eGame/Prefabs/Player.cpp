#include "Player.h"

#include <d2e/Core/Random.h>

#include <d2e/Physics/CollisionInfo.h>

#include <d2e/ES/Components/Tag.h>
#include <d2e/Es/Components/Movement.h>
#include <d2e/ES/Components/RigidBody.h>
#include <d2e/ES/Components/Transform.h>
#include <d2e/ES/Components/CircleSprite.h>
#include <d2e/ES/Components/CircleCollider.h>

#include <d2eNet/Core/Client.h>
#include <d2eNet/Core/Packet.h>

#include "Systems/SplatAnimationManager.h"


namespace d2eGame
{

void Player::CreatePrefab(d2e::WeakRef<d2e::Scene> scene, const bool isPlayer1)
{
    const sf::Color playerColor = isPlayer1 ? PLAYER_1_COLOR : PLAYER_2_COLOR;

    mGameObject = scene->CreateGameObject();
    mGameObject->SetId(isPlayer1 ? d2e::Engine::PLAYER_ONE_ULID : d2e::Engine::PLAYER_TWO_ULID);

    constexpr float PLAYER_RADIUS = 0.2f;

    mGameObject->AddComponent<d2e::Tag>()->tag = d2e::ComponentTag::PLAYER;

    d2e::WeakRef<d2e::Transform> transform = mGameObject->GetComponent<d2e::Transform>();
    transform->translation = isPlayer1 ? d2e::Vec2{ -4.5f, -1.5f } : d2e::Vec2{ 4.5f, 3.5f };
    transform->SetSyncValuesOnUpdate(true);

    d2e::WeakRef<d2e::CircleSprite> visual = mGameObject->AddComponent<d2e::CircleSprite>();
    visual->SetColor(playerColor);
    visual->SetRadius(PLAYER_RADIUS);

    d2e::WeakRef<d2e::CircleCollider> collider = mGameObject->AddComponent<d2e::CircleCollider>();
    collider->SetRadius(PLAYER_RADIUS);
    collider->SetOnCollisionEnterCallback([](const d2e::CollisionInfo& info)
    {
        // If the player collides with the wall, we don't need to add the splat.
        if (auto tag = info.other->GetComponent<d2e::Tag>(); tag.IsRefValid() && (tag->tag == d2e::ComponentTag::WALL || tag->tag == d2e::ComponentTag::BULLET))
        {
            return;
        }

        // If the player collides with anything other than the top of the floor, don't add the splat.
        if (info.collisionPosition.y >= info.instance->GetComponent<d2e::Transform>()->translation.y)
        {
            return;
        }

        SplatAnimationManager::Instance()->AddSplatAnimation(info.collisionPosition, info.instance->GetId() == d2e::Engine::PLAYER_ONE_ULID);
    });

    mGameObject->AddComponent<d2e::Movement>();

    d2e::WeakRef<d2e::RigidBody> rb = mGameObject->AddComponent<d2e::RigidBody>();
    rb->SetGravity(d2e::Vec2{ 0.0f, -15.0f });
    rb->SetRestitution(0.1f);
    rb->SetSyncValuesOnUpdate(true);

    mBulletManager.Init(scene, mGameObject);
}

void Player::SyncPlayer()
{
    d2eNet::Packet packet;
    const uint64_t id = mGameObject->GetId();

    packet.AddLineWithId(id);
    packet.AddType<d2e::Tag>(id, mGameObject->GetComponent<d2e::Tag>()->Serialize());
    packet.AddType<d2e::Transform>(id, mGameObject->GetComponent<d2e::Transform>()->Serialize());
    packet.AddType<d2e::CircleSprite>(id, mGameObject->GetComponent<d2e::CircleSprite>()->Serialize());
    packet.AddType<d2e::CircleCollider>(id, mGameObject->GetComponent<d2e::CircleCollider>()->Serialize());
    packet.AddType<d2e::Movement>(id, mGameObject->GetComponent<d2e::Movement>()->Serialize());
    packet.AddType<d2e::RigidBody>(id, mGameObject->GetComponent<d2e::RigidBody>()->Serialize());

    packet.AddSyncObject(id);

    d2e::Engine::Instance()->GetClient()->AddPacketToSend(packet);
}
} // Namespace d2eGame.