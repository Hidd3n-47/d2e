#include "d2ePch.h"
#include "BoxCollider.h"

#include "Es/GameObject.h"
#include "ES/Scene.h"

namespace d2e
{

void BoxCollider::OnComponentAdded(const WeakRef<GameObject> parent)
{
    parent->GetScene()->GetCollisionHandler()->AddBox(parent);

    IComponent::OnComponentAdded(parent);
}


#ifdef DEV_CONFIGURATION
void BoxCollider::Render(WeakRef<sf::RenderWindow> window)
{
    const auto transform = mParent->GetComponent<Transform>();

    mDebugRender.setPosition({ transform->translation.x - mHalfExtents.x, transform->translation.y - mHalfExtents.y });
    mDebugRender.setSize({ mHalfExtents.x * 2.0f, mHalfExtents.y * 2.0f });
    mDebugRender.setFillColor(sf::Color::Transparent);
    mDebugRender.setOutlineColor(sf::Color::White);
    mDebugRender.setOutlineThickness(2.0f);
    window->draw(mDebugRender);
}
#endif // DEV_CONFIGURATION.

} // Namespace d2e.
