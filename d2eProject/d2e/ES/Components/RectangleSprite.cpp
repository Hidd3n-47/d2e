#include "d2ePch.h"
#include "RectangleSprite.h"

#include "ES/GameObject.h"

namespace d2e
{

void RectangleSprite::OnComponentAdded(const WeakRef<GameObject> parent)
{
    IComponent::OnComponentAdded(parent);
}

void RectangleSprite::Update(const float dt)
{
    mParent->GetComponent<Transform>()->translation = mParent->GetComponent<Transform>()->translation + Vec2{ 10.f * dt, 10.0f * dt };
}

void RectangleSprite::Render(WeakRef<sf::RenderWindow> window)
{
    const auto transform = mParent->GetComponent<Transform>();

    mRectangle.setPosition({ transform->translation.x, transform->translation.y });
    window->draw(mRectangle);
}

} // Namespace d2e.
