#include "d2ePch.h"
#include "CircleSprite.h"

#include "ES/GameObject.h"

namespace d2e
{

void CircleSprite::OnComponentAdded(const WeakRef<GameObject> parent)
{
    IComponent::OnComponentAdded(parent);
}

void CircleSprite::Render(WeakRef<sf::RenderWindow> window)
{
    const auto transform = mParent->GetComponent<Transform>();

    mCircle.setPosition({ transform->translation.x, transform->translation.y });
    mCircle.setRadius(mRadius);
    window->draw(mCircle);
}

} // Namespace d2e.
