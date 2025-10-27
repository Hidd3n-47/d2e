#include "d2ePch.h"
#include "CircleCollider.h"

#include "ES/Scene.h"
#include "Es/GameObject.h"
#include "Physics/CollisionInfo.h"
#include "Es/Components/Transform.h"

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

void CircleCollider::OnCollisionEnter(const CollisionInfo& info) const
{
    if (mOnCollisionEnterCallback)
    {
        mOnCollisionEnterCallback(info);
    }
}

void CircleCollider::UpdateObjectsCollidedWith(const std::vector<CollisionInfo>& collisionInfos)
{
    std::set<WeakRef<GameObject>> thisFrameCollidedWithObjects;
    for (const CollisionInfo collisionInfo : collisionInfos)
    {
        thisFrameCollidedWithObjects.emplace(collisionInfo.other);

        if (mObjectsCollidingWith.contains(collisionInfo.other))
        {
            continue;
        }

        OnCollisionEnter(collisionInfo);
    }

    mObjectsCollidingWith =  thisFrameCollidedWithObjects;
    mCollidedLastFrame    = !mObjectsCollidingWith.empty();
}

std::string CircleCollider::Serialize() const
{
    return SerializeUtils::Serialize(mSyncValuesOnUpdate) +
           SerializeUtils::Serialize(mCollidedLastFrame) +
           SerializeUtils::Serialize(mRadius);
}

void CircleCollider::Deserialize(const std::string& string)
{
    SerializeUtils::Deserialize(mSyncValuesOnUpdate, std::string{ string[0] });
    SerializeUtils::Deserialize(mCollidedLastFrame, std::string{ string[1] });
    SerializeUtils::Deserialize(mRadius, string.substr(2));
}

#ifdef DEV_CONFIGURATION
void CircleCollider::Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera)
{
    if (!mEnabled)
    {
        return;
    }

    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    const Vec2 position{ camera.PositionToScreenSpace(transform->translation + Vec2{ -mRadius, mRadius }) };
    const Vec2 size{ camera.SizeInScreenSpace(Vec2{ mRadius, 0.0f }) };

    mDebugRender.setPosition({ position.x, position.y });
    mDebugRender.setRadius(size.x);
    mDebugRender.setFillColor(sf::Color::Transparent);
    mDebugRender.setOutlineColor(sf::Color::White);
    mDebugRender.setOutlineThickness(2.0f);
    window->draw(mDebugRender);
}
#endif // DEV_CONFIGURATION.

} // Namespace d2e.
