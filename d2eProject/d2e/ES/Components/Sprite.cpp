#include "d2ePch.h"
#include "Sprite.h"

#include "ES/GameObject.h"
#include "Es/Components/Transform.h"

#include "Rendering/SpriteManager.h"

namespace d2e
{

Sprite::Sprite(const spriteId spriteId)
    : mSpriteId(spriteId)
{
    // Empty.
}

Sprite::Sprite(const std::filesystem::path& filepath)
    : mSpriteId(SpriteManager::Instance()->LoadTexture(filepath))
{
    // Empty.
}

void Sprite::Update(const float dt)
{

}

void Sprite::Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera)
{
    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    WeakRef<SpriteInfo> sprite = SpriteManager::Instance()->GetSprite(mSpriteId);

    const sf::Vector2u textureSize = sprite->texture.getSize();

    Vec2 screenCoords = camera.PositionToScreenSpace(Vec2{ transform->translation.x , transform->translation.y });
    sprite->sprite.setPosition({ screenCoords.x - static_cast<float>(textureSize.x) * 0.5f * transform->scale.x, screenCoords.y - static_cast<float>(textureSize.y) * 0.5f * transform->scale.y });
    sprite->sprite.setScale({ transform->scale.x, transform->scale.y });
    window->draw(sprite->sprite);
}

} // Namespace d2e.
