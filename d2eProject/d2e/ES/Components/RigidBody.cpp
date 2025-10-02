#include "d2ePch.h"
#include "RigidBody.h"

#include "ES/Scene.h"

namespace d2e
{

void RigidBody::OnComponentAdded(const WeakRef<GameObject> parent)
{
    parent->GetScene()->GetPhysicsManager()->AddBody(parent);

    IComponent::OnComponentAdded(parent);
}

void RigidBody::OnComponentRemoved()
{
    mParent->GetScene()->GetPhysicsManager()->RemoveBody(mParent);
}

} // Namespace d2e.
