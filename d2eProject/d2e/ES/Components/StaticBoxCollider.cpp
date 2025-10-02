#include "d2ePch.h"
#include "StaticBoxCollider.h"

#include "ES/Scene.h"
#include "Es/GameObject.h"

namespace d2e
{

void StaticBoxCollider::OnComponentAdded(const WeakRef<GameObject> parent)
{
    parent->GetScene()->GetCollisionHandler()->AddStaticBox(parent);

    IComponent::OnComponentAdded(parent);
}

void StaticBoxCollider::OnComponentRemoved()
{
    mParent->GetScene()->GetCollisionHandler()->RemoveCircle(mParent);
}

#ifdef DEV_CONFIGURATION
void StaticBoxCollider::Render(WeakRef<sf::RenderWindow> window)
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
