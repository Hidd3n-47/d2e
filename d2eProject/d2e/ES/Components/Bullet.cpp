#include "d2ePch.h"
#include "Bullet.h"

#include "ES/GameObject.h"

#include "CircleCollider.h"
#include "CircleSprite.h"
#include "RigidBody.h"

namespace d2e
{

void Bullet::Update(const float dt)
{
    WeakRef<RigidBody> rb = mParent->GetComponent<RigidBody>();

    WeakRef<CircleCollider> collider = mParent->GetComponent<CircleCollider>();
    WeakRef<CircleSprite> circleSprite = mParent->GetComponent<CircleSprite>();


    const bool enabled = rb->IsEnabled();

    collider->SetEnabled(enabled);
    circleSprite->SetEnabled(enabled);
}

} // Namespace d2e.