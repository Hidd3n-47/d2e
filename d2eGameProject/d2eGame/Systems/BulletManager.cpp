#include "BulletManager.h"

#include <d2e/ES/Scene.h>
#include <d2e/Core/Engine.h>

#include <d2e/ES/Components/Transform.h>
#include <d2e/ES/Components/RigidBody.h>
#include <d2e/ES/Components/CircleSprite.h>
#include <d2e/ES/Components/CircleCollider.h>

#include <d2eNet/Core/Client.h>
#include <d2eNet/Core/Packet.h>

#include "d2e/ES/Components/Tag.h"
#include "d2e/Physics/CollisionInfo.h"
#include "src/Defines.h"

namespace d2eGame
{

void BulletManager::Init(d2e::WeakRef<d2e::Scene> scene, const d2e::WeakRef<d2e::GameObject> player)
{
    mPlayer = player;

    const uint64_t playerId = mPlayer->GetId();

    for (uint64_t i{ 0 }; i < NUM_BULLETS_IN_POOL; ++i)
    {
        mPool.emplace_back(scene->CreateGameObject());
        d2e::WeakRef<d2e::GameObject> bullet = mPool.back();
        bullet->SetId(d2e::Ulid{ d2e::Engine::BULLET_POOL_STARTING_ULID * playerId + i});

        d2e::WeakRef<d2e::Tag> tag = bullet->AddComponent<d2e::Tag>();
        tag->tag = d2e::ComponentTag::BULLET;

        constexpr float radius = 0.05f;
        d2e::WeakRef<d2e::CircleSprite> sprite = bullet->AddComponent<d2e::CircleSprite>();
        sprite->SetRadius(radius);
        sprite->SetEnabled(false);
        sprite->SetSyncValuesOnUpdate(false);
        d2e::WeakRef<d2e::CircleCollider> collider = bullet->AddComponent<d2e::CircleCollider>();
        collider->SetRadius(radius);
        collider->SetEnabled(false);
        sprite->SetSyncValuesOnUpdate(false);
        d2e::WeakRef<d2e::RigidBody> rigidBody = bullet->AddComponent<d2e::RigidBody>();
        rigidBody->SetGravity(d2e::Vec2{ 0.0f, -8.0f });
        rigidBody->SetEnabled(false);
        sprite->SetSyncValuesOnUpdate(true);

        collider->SetOnCollisionEnterCallback([](const d2e::CollisionInfo& info)
        {
            if (auto tag = info.other->GetComponent<d2e::Tag>(); tag.IsRefValid() && tag->tag == d2e::ComponentTag::PLAYER)
            {
                // Prevent the bullet from colliding with the player that shot it.
                if (info.instance->GetId() / 100u == info.other->GetId())
                {
                    return;
                }

                // Player shot the other player.
                GAME_LOG("Player shot the other player");

                d2e::WeakRef<GameScene> gameScene = GameManager::Instance()->GetScene().Cast<GameScene>();
                gameScene->IncreaseScore(info.other->GetId());
                gameScene->ChangeGameState(GameState::PLAYER_KILLED);
                return;
            }

            d2e::WeakRef<d2e::RigidBody> rigidBody = info.instance->GetComponent<d2e::RigidBody>();
            rigidBody->SetEnabled(false);
            info.instance->GetComponent<d2e::CircleSprite>()->SetEnabled(false);
            info.instance->GetComponent<d2e::CircleCollider>()->SetEnabled(false);

            d2eNet::Packet packet;
            packet.UpdateType<d2e::RigidBody>(info.instance->GetId(), rigidBody->Serialize());

            d2e::WeakRef<d2eNet::Client> client = d2e::Engine::Instance()->GetClient();
            client->AddPacketToSend(packet);
        });

        d2e::WeakRef<d2eNet::Client> client = d2e::Engine::Instance()->GetClient();
        if (client->GetId() == 1)
        {
            const uint64_t bulletId = bullet->GetId();
            d2eNet::Packet packet;
            packet.AddLineWithId(bulletId);
            packet.AddType<d2e::Tag>(bulletId, tag->Serialize());
            packet.AddType<d2e::CircleCollider>(bulletId, collider->Serialize());
            packet.AddType<d2e::RigidBody>(bulletId, rigidBody->Serialize());
            packet.AddSyncObject(bulletId);
            client->AddPacketToSend(packet);
        }
    }
}

void BulletManager::ShootBullet(const d2e::Vec2 direction)
{
    d2e::WeakRef<d2e::GameObject> bullet = mPool[mPoolIndex];

    const d2e::Vec2 spawnPoint = mPlayer->GetComponent<d2e::Transform>()->translation + direction * OFFSET;

    d2e::WeakRef<d2e::RigidBody> rigidBody = bullet->GetComponent<d2e::RigidBody>();

    bullet->GetComponent<d2e::CircleCollider>()->SetEnabled(true);
    bullet->GetComponent<d2e::CircleSprite>()->SetEnabled(true);
    rigidBody->SetEnabled(true);

    bullet->GetComponent<d2e::Transform>()->translation = spawnPoint;

    constexpr float impulse = 12.0f;
    rigidBody->AddVelocity(direction * impulse);

    d2eNet::Packet packet;
    packet.UpdateType<d2e::RigidBody>(bullet->GetId(), rigidBody->Serialize());

    d2e::WeakRef<d2eNet::Client> client = d2e::Engine::Instance()->GetClient();
    client->AddPacketToSend(packet);

    mPoolIndex = (mPoolIndex + 1) % NUM_BULLETS_IN_POOL;
}

} // Namespace d2eGame.