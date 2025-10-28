#include "d2ePch.h"
#include "Bullet.h"

#include "ES/GameObject.h"

#include "CircleCollider.h"
#include "CircleSprite.h"
#include "RigidBody.h"
#include "Math/Math.h"

namespace d2e
{

void Bullet::Update(const float dt)
{
    WeakRef<RigidBody> rb = mParent->GetComponent<RigidBody>();

    WeakRef<CircleCollider> collider = mParent->GetComponent<CircleCollider>();
    WeakRef<CircleSprite> circleSprite = mParent->GetComponent<CircleSprite>();

    const bool enabled = !Math::IsEqualTo(rb->GetVelocity().MagnitudeSquared(), 0.0f, 0.02f);

    collider->SetEnabled(enabled);
    circleSprite->SetEnabled(enabled);
}

} // Namespace d2e.