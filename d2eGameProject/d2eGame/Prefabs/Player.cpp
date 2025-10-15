#include "Player.h"

#include <random>

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

void Player::CreatePrefab(d2e::WeakRef<d2e::Scene> scene)
{
    d2eNet::Packet packet;
    mGameObject = scene->CreateGameObject();
    const uint32_t id = mGameObject->GetId();
    packet.AddLineWithId(id);

    //todo this should be changed into a percentage and not a pixel based.
    constexpr float PLAYER_RADIUS = 20.0f;

    auto transform = mGameObject->GetComponent<d2e::Transform>();
    transform->translation = d2e::Vec2{ 1000.0f, 100.0f };
    packet.AddType<d2e::Transform>(id, transform->Serialize());

    auto visual = mGameObject->AddComponent<d2e::CircleSprite>();
    visual->SetColor(sf::Color{255, 0, 132, 255 });
    visual->SetRadius(PLAYER_RADIUS);
    packet.AddType<d2e::CircleSprite>(id, visual->Serialize());

    static d2e::spriteId spriteId = d2e::SpriteManager::Instance()->LoadTexture("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/SplatAnim/SplatSpritesheet.png");

    auto collider = mGameObject->AddComponent<d2e::CircleCollider>();
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

        //todo refine the anim comp below and remove rand next line.
        //todo need to pool this as we cannot have infinite splats.
        const float r = d2e::Random::GetRandomBetween<int>(0, 2) == 1 ? 1.0f : -1.0f;

        auto object = info.instance->GetScene()->CreateGameObject();
        auto anim = object->AddComponent<d2e::Animation>();
        const d2e::AnimationDetails animDetails
        {
            .spriteSheetId      = spriteId,
            .framesHorizontal   = 4,
            .frameCount         = 7,
            .repeatAnimation    = false
        };

        //todo change this from taking in a filepath to taking in a sprite id which is obtained from a sprite manager.
        anim->CreateAnimation(animDetails, 0.015f);
        anim->SetSpriteColor(sf::Color{ 255, 0, 132, 255 });
        auto transform = object->GetComponent<d2e::Transform>();
        transform->translation = info.collisionPosition - d2e::Vec2{ 0.0f, -0.5f };
        transform->scale = d2e::Vec2{ 0.13f * r, 0.13f };
    });
    packet.AddType<d2e::CircleCollider>(id, collider->Serialize());

    //packet.AddType<d2e::Movement>();
    auto movement = mGameObject->AddComponent<d2e::Movement>();
    movement->SetSpeed(800.0f);

    auto rb = mGameObject->AddComponent<d2e::RigidBody>();
    rb->SetGravity(d2e::Vec2{ 0.0f, 15.0f });
    rb->SetRestitution(0.1f);
    packet.AddType<d2e::RigidBody>(id, rb->Serialize());

    packet.AddSyncObject(id);

    d2e::Engine::Instance()->GetClient()->AddPacketToSend(packet);
}

} // Namespace d2eGame.