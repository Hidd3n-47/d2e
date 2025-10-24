#include "d2ePch.h"
#include "StaticBoxCollider.h"

#include "ES/Scene.h"
#include "Es/GameObject.h"
#include "Es/Components/Transform.h"

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
void StaticBoxCollider::Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera)
{
    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    const Vec2 position{ camera.PositionToScreenSpace(transform->translation + Vec2{  -mHalfExtents.x, +mHalfExtents.y }) };
    const Vec2 size{ camera.SizeInScreenSpace(Vec2{ mHalfExtents.x * 2.0f, mHalfExtents.y * 2.0f }) };

    mDebugRender.setPosition({ position.x, position.y });
    mDebugRender.setSize({ size.x, size.y });
    mDebugRender.setFillColor(sf::Color::Transparent);
    mDebugRender.setOutlineColor(sf::Color::White);
    mDebugRender.setOutlineThickness(2.0f);
    window->draw(mDebugRender);
}
#endif // DEV_CONFIGURATION.

} // Namespace d2e.
