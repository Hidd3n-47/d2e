#include "SplatAnimationManager.h"

#include <SFML/Graphics/Color.hpp>

#include <d2e/Core/Random.h>

#include <d2e/ES/Scene.h>
#include <d2e/ES/Components/Animation.h>
#include <d2e/ES/Components/Transform.h>

#include "Prefabs/Player.h"

namespace d2eGame
{

std::unique_ptr<SplatAnimationManager> SplatAnimationManager::mInstance = std::make_unique<SplatAnimationManager>();

void SplatAnimationManager::InitPool(d2e::WeakRef<d2e::Scene> scene)
{
    mPool.reserve(POOL_SIZE);

    for (uint32_t i{ 0 }; i < POOL_SIZE; ++i)
    {
        mPool.emplace_back(scene->CreateGameObject());
    }

    mSpriteId = d2e::SpriteManager::Instance()->LoadTexture("Assets/SplatSpritesheet.png");
}

void SplatAnimationManager::AddSplatAnimation(const d2e::Vec2 collisionPosition, const bool playerOne)
{
    const sf::Color color = playerOne ? Player::PLAYER_1_COLOR : Player::PLAYER_2_COLOR;

    d2e::WeakRef<d2e::GameObject> object = mPool[mCurrentIndex];
    if (d2e::WeakRef<d2e::Animation> anim = object->GetComponent<d2e::Animation>(); anim.IsRefValid())
    {
        anim->RestartAnimation();

        SetTransformForAnimation(object, collisionPosition);

        mCurrentIndex = (mCurrentIndex + 1) % POOL_SIZE;

        return;
    }

    d2e::WeakRef<d2e::Animation> anim = object->AddComponent<d2e::Animation>();

    const d2e::AnimationDetails animDetails
    {
        .spriteSheetId      = mSpriteId,
        .framesHorizontal   = 4,
        .frameCount         = 7,
        .repeatAnimation    = false
    };
    anim->CreateAnimation(animDetails, 0.015f);
    anim->SetSpriteColor(color);

    SetTransformForAnimation(object, collisionPosition);

    mCurrentIndex = (mCurrentIndex + 1) % POOL_SIZE;
}

void SplatAnimationManager::SetTransformForAnimation(d2e::WeakRef<d2e::GameObject> gameObject, const d2e::Vec2 position)
{
    constexpr d2e::Vec2 offset{ 0.0f, -0.02f };
    constexpr d2e::Vec2 scale{ 0.13f,  0.13f };

    const float randomFlip = d2e::Random::GetRandomBetween<int>(0, 2) == 1 ? 1.0f : -1.0f;

    d2e::WeakRef<d2e::Transform> transform = gameObject->GetComponent<d2e::Transform>();

    transform->translation = position + offset;
    transform->scale       = scale * d2e::Vec2{ randomFlip, 1.0f };
}

} // Namespace d2eGame.
