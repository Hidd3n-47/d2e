#include "d2ePch.h"
#include "RectangleSprite.h"

#include "ES/GameObject.h"

namespace d2e
{

void RectangleSprite::OnComponentAdded(const WeakRef<GameObject> parent)
{
    IComponent::OnComponentAdded(parent);
}

void RectangleSprite::Render(WeakRef<sf::RenderWindow> window)
{
    const auto transform = mParent->GetComponent<Transform>();

    mRectangle.setPosition({ transform->translation.x, transform->translation.y });
    mRectangle.setSize({ mSize.x, mSize.y });
    window->draw(mRectangle);
}

} // Namespace d2e.
