#include "d2ePch.h"
#include "CircleCollider.h"

#include "ES/Scene.h"
#include "Es/GameObject.h"

namespace d2e
{

void CircleCollider::OnComponentAdded(const WeakRef<GameObject> parent)
{
    parent->GetScene()->GetCollisionHandler()->AddCircle(parent);

    IComponent::OnComponentAdded(parent);
}

void CircleCollider::OnComponentRemoved()
{
    mParent->GetScene()->GetCollisionHandler()->RemoveCircle(mParent);
}

#ifdef DEV_CONFIGURATION
void CircleCollider::Render(WeakRef<sf::RenderWindow> window)
{
    const auto transform = mParent->GetComponent<Transform>();

    mDebugRender.setPosition({ transform->translation.x - mRadius, transform->translation.y - mRadius });
    mDebugRender.setRadius(mRadius);
    mDebugRender.setFillColor(sf::Color::Transparent);
    mDebugRender.setOutlineColor(sf::Color::White);
    mDebugRender.setOutlineThickness(2.0f);
    window->draw(mDebugRender);
}
#endif // DEV_CONFIGURATION.

} // Namespace d2e.
