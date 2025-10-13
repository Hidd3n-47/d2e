#include "d2ePch.h"
#include "PhysicsManager.h"

#include "ES/GameObject.h"
#include "ES/Components/RigidBody.h"
#include "Es/Components/Transform.h"

namespace d2e
{

void PhysicsManager::Update(const float dt) const
{
    for (const WeakRef<GameObject> body : mBodies)
    {
        WeakRef<Transform> transform = body->GetComponent<Transform>();
        WeakRef<RigidBody> rigidBody = body->GetComponent<RigidBody>();

        const Vec2 acceleration = (rigidBody->mForce + rigidBody->mGravity) / rigidBody->mMass * dt;

        rigidBody->SetVelocity(rigidBody->mVelocity + acceleration);

        transform->translation += rigidBody->mVelocity;

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