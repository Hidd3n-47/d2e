#include "d2ePch.h"
#include "PhysicsManager.h"

#include "Core/Engine.h"
#include "ES/GameObject.h"
#include "ES/Components/RigidBody.h"

namespace d2e
{

void PhysicsManager::Update(const float dt) const
{
    for (const WeakRef<GameObject> body : mBodies)
    {
        WeakRef<Transform> transform = body->GetComponent<Transform>();
        WeakRef<RigidBody> rigidBody = body->GetComponent<RigidBody>();

        //todo remove this if we not going to use drag.
        constexpr float dragCoefficient = 0.00f;
        const Vec2 drag = rigidBody->mVelocity * -dragCoefficient;
        const Vec2 acceleration = (rigidBody->mForce + rigidBody->mGravity + drag) / rigidBody->mMass * dt;

        rigidBody->mVelocity += acceleration;
        Vec2::Min(rigidBody->mMaxSpeed, rigidBody->mVelocity); // todo fix this as this doesn't work when not going in basis vector directions

        transform->translation += rigidBody->mVelocity;

        DEBUG_LOG("{}-{}", rigidBody->mVelocity.x, rigidBody->mVelocity.y);
        rigidBody->mForce = Vec2{ 0.0f, 0.0f };
    }
}

void PhysicsManager::AddBody(const WeakRef<GameObject> body)
{
    mBodies.emplace_back(body);
}

void PhysicsManager::RemoveBody(const WeakRef<GameObject> body)
{
    for (auto b = mBodies.begin(); b != mBodies.end(); ++b)
    {
        if (*b == body)
        {
            mBodies.erase(b);
            return;
        }
    }
}

} // Namespace d2e.