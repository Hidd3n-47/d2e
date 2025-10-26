#include "d2ePch.h"
#include "UiButton.h"

#include "Core/Engine.h"
#include "ES/GameObject.h"
#include "Input/InputManager.h"
#include "Es/Components/Transform.h"

namespace d2e
{

UiButton::UiButton(const spriteId spriteId)
    : mSpriteId(spriteId)
{
    // Empty.
}

UiButton::UiButton(const spriteId spriteId, const std::function<void()>& callback)
    : mSpriteId(spriteId)
    , mOnButtonPressed(callback)
{
    // Empty.
}

void UiButton::Update(const float dt)
{
    const Vec2 mousePosition = Engine::Instance()->GetInputManager()->GetMousePosition();
    const OrthoCamera camera = Engine::Instance()->GetCamera();

    const Vec2 translation = mParent->GetComponent<Transform>()->translation;

    const sf::Vector2u textureSize = SpriteManager::Instance()->GetSprite(mSpriteId)->texture.getSize();

    const Vec2 delta = camera.PositionToScreenSpace(translation) - mousePosition;
    if (abs(delta.x) < static_cast<float>(textureSize.x) && abs(delta.y) < static_cast<float>(textureSize.y) )
    {
        if (mOnButtonPressed && Engine::Instance()->GetInputManager()->IsMousePressed(sf::Mouse::Button::Left))
        {
            mOnButtonPressed();
        }
    }
}

void UiButton::Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera)
{
    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    WeakRef<SpriteInfo> sprite = SpriteManager::Instance()->GetSprite(mSpriteId);

    const sf::Vector2u textureSize = sprite->texture.getSize();

    Vec2 screenCoords = camera.PositionToScreenSpace(Vec2{ transform->translation.x , transform->translation.y });
    sprite->sprite.setPosition({ screenCoords.x - static_cast<float>(textureSize.x) * 0.5f, screenCoords.y - static_cast<float>(textureSize.y) * 0.5f });
    window->draw(sprite->sprite);
}

} // Namespace d2e.
