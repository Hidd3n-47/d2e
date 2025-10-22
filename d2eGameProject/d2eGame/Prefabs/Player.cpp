#include "Player.h"

#include <d2e/Core/Random.h>

#include <d2e/Physics/CollisionInfo.h>

#include <d2e/ES/Components/Tag.h>
#include <d2e/Es/Components/Movement.h>
#include <d2e/Es/Components/Animation.h>
#include <d2e/ES/Components/RigidBody.h>
#include <d2e/ES/Components/Transform.h>
#include <d2e/ES/Components/CircleSprite.h>
#include <d2e/ES/Components/CircleCollider.h>

#include <d2eNet/Core/Client.h>
#include <d2eNet/Core/Packet.h>


namespace d2eGame
{

void Player::CreatePrefab(d2e::WeakRef<d2e::Scene> scene, const bool isPlayer1)
{
    const sf::Color playerColor = isPlayer1 ? PLAYER_1_COLOR : PLAYER_2_COLOR;

    d2eNet::Packet packet;
    mGameObject = scene->CreateGameObject();
    const d2e::Ulid id = mGameObject->GetId();
    packet.AddLineWithId(id);

    //todo this should be changed into a percentage and not a pixel based.
    constexpr float PLAYER_RADIUS = 20.0f;

    d2e::WeakRef<d2e::Transform> transform = mGameObject->GetComponent<d2e::Transform>();
    transform->translation = isPlayer1 ? d2e::Vec2{ 900.0f, 100.0f } : d2e::Vec2{ 1100.0f, 100.0f };
    transform->SetSyncValuesOnUpdate(true);
    packet.AddType<d2e::Transform>(id, transform->Serialize());

    d2e::WeakRef<d2e::CircleSprite> visual = mGameObject->AddComponent<d2e::CircleSprite>();
    visual->SetColor(playerColor);
    visual->SetRadius(PLAYER_RADIUS);
    packet.AddType<d2e::CircleSprite>(id, visual->Serialize());

    static d2e::spriteId spriteId = d2e::SpriteManager::Instance()->LoadTexture("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/SplatAnim/SplatSpritesheet.png");

    d2e::WeakRef<d2e::CircleCollider> collider = mGameObject->AddComponent<d2e::CircleCollider>();
    collider->SetRadius(PLAYER_RADIUS);
    collider->SetOnCollisionEnterCallback([&](const d2e::CollisionInfo& info)
    {
        // If the player collides with the wall, we don't need to add the splat.
        if (auto tag = info.other->GetComponent<d2e::Tag>(); tag.IsRefValid() && tag->tag == d2e::ComponentTag::WALL)
        {
            return;
        }

        // If the player collides with anything other than the top of the floor, don't add the splat.
        if (info.collisionPosition.y <= info.instance->GetComponent<d2e::Transform>()->translation.y)
        {
            return;
        }

        //todo need to pool this as we cannot have infinite splats.
        const float r = d2e::Random::GetRandomBetween<int>(0, 2) == 1 ? 1.0f : -1.0f;

        d2e::WeakRef<d2e::GameObject> object = info.instance->GetScene()->CreateGameObject();
        d2e::WeakRef<d2e::Animation>  anim   = object->AddComponent<d2e::Animation>();
        const d2e::AnimationDetails animDetails
        {
            .spriteSheetId      = spriteId,
            .framesHorizontal   = 4,
            .frameCount         = 7,
            .repeatAnimation    = false
        };

        anim->CreateAnimation(animDetails, 0.015f);
        anim->SetSpriteColor(PLAYER_1_COLOR);
        d2e::WeakRef<d2e::Transform> t = object->GetComponent<d2e::Transform>();
        t->translation = info.collisionPosition - d2e::Vec2{ 0.0f, -0.5f };
        t->scale = d2e::Vec2{ 0.13f * r, 0.13f };
    });
    packet.AddType<d2e::CircleCollider>(id, collider->Serialize());

    d2e::WeakRef<d2e::Movement> movement = mGameObject->AddComponent<d2e::Movement>();
    movement->speed = 800.0f;
    packet.AddType<d2e::Movement>(id, movement->Serialize());

    d2e::WeakRef<d2e::RigidBody> rb = mGameObject->AddComponent<d2e::RigidBody>();
    rb->SetGravity(d2e::Vec2{ 0.0f, 15.0f });
    rb->SetRestitution(0.1f);
    rb->SetSyncValuesOnUpdate(true);
    packet.AddType<d2e::RigidBody>(id, rb->Serialize());

    packet.AddSyncObject(id);

    d2e::Engine::Instance()->GetClient()->AddPacketToSend(packet);
}

} // Namespace d2eGame.