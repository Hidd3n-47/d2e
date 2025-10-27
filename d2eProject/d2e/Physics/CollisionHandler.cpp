#include "d2ePch.h"
#include "CollisionHandler.h"

#include "ES/GameObject.h"
#include "Physics/CollisionInfo.h"
#include "Es/Components/Transform.h"
#include "ES/Components/StaticBoxCollider.h"
#include "ES/Components/CircleCollider.h"
#include "ES/Components/RigidBody.h"
#include "Math/Math.h"

namespace d2e
{

void CollisionHandler::Update() const
{
    for (const WeakRef<GameObject> circle1 : mCircleColliders)
    {
        std::vector<CollisionInfo> collisionInfos;
        for (const WeakRef<GameObject> circle2 : mCircleColliders)
        {
            if (circle1 == circle2)
            {
                continue;
            }

            ResolveCollisionBetweenCircles(collisionInfos, circle1, circle2);
        }
        circle1->GetComponent<CircleCollider>()->UpdateObjectsCollidedWith(collisionInfos);
    }

    for (const WeakRef<GameObject> circle : mCircleColliders)
    {
        WeakRef<CircleCollider> circleCollider = circle->GetComponent<CircleCollider>();

        std::vector<CollisionInfo> collisionInfos;
        for (const WeakRef<GameObject> box : mStaticBoxColliders)
        {
            ResolveCollisionBetweenBoxAndCircle(collisionInfos, box, circle, circleCollider);
        }
        circleCollider->UpdateObjectsCollidedWith(collisionInfos);
    }
}

void CollisionHandler::AddStaticBox(const WeakRef<GameObject> box)
{
    mStaticBoxColliders.emplace_back(box);
}

void CollisionHandler::AddCircle(const WeakRef<GameObject> circle)
{
    mCircleColliders.emplace_back(circle);
}

void CollisionHandler::RemoveStaticBox(const WeakRef<GameObject> box)
{
    for (auto it{ mStaticBoxColliders.begin() }; it != mStaticBoxColliders.end(); ++it)
    {
        if (*it == box)
        {
            mStaticBoxColliders.erase(it);
            return;
        }
    }
}

void CollisionHandler::RemoveCircle(const WeakRef<GameObject> circle)
{
    for (auto it{ mCircleColliders.begin() }; it != mCircleColliders.end(); ++it)
    {
        if (*it == circle)
        {
            mCircleColliders.erase(it);
            return;
        }
    }
}

void CollisionHandler::ResolveCollisionBetweenCircles(std::vector<CollisionInfo>& collisionInfos, WeakRef<GameObject> circle1, WeakRef<GameObject> circle2)
{
    WeakRef<CircleCollider> circleCollider  = circle1->GetComponent<CircleCollider>();
    WeakRef<CircleCollider> circle2Collider = circle2->GetComponent<CircleCollider>();

    if (!circleCollider->IsEnabled() || !circle2Collider->IsEnabled())
    {
        return;
    }

    const float circle1Radius   = circleCollider->GetRadius();
    const Vec2  circle1Centre   = circle1->GetComponent<Transform>()->translation;

    const float circle2Radius   = circle2Collider->GetRadius();
    const Vec2  circle2Centre   = circle2->GetComponent<Transform>()->translation;

    Vec2 delta = circle2Centre - circle1Centre;
    const float length = delta.Magnitude();

    const float minDistance = circle1Radius + circle2Radius;

    if (length >= minDistance)
    {
        return;
    }

    if (!Math::IsEqualTo(length, 0.0f))
    {
        // Normalize delta so we only take into account the direction between the centres.
        // Note we divide by length here as its more efficient than calling Normalize since we already computed the sqrt length.
        delta /= length;
    }

    const float overlap = minDistance - length;
    const float halfOverlap = (minDistance - length) * 0.5f;

    collisionInfos.emplace_back(circle1, circle2, circle1Centre + delta * circle1Radius);

    circle1->GetComponent<Transform>()->translation +=  delta * halfOverlap * -1.0f;
    circle2->GetComponent<Transform>()->translation +=  delta * halfOverlap;

    //WeakRef<RigidBody> circleOneRigidBody = circle1->GetComponent<RigidBody>();
    //WeakRef<RigidBody> circleTwoRigidBody = circle2->GetComponent<RigidBody>();

    //// Correct the position slightly to prevent jittering.
    //constexpr float PERCENT = 0.8f;
    //constexpr float SLOP = 0.01f;
    //{
    //    circle1->GetComponent<Transform>()->translation += delta * (overlap - SLOP) * PERCENT;

    //    const float velAlongNormal = Vec2::Dot(circleOneRigidBody->mVelocity, delta);

    //    float j1 = -(1.0f + circleOneRigidBody->mRestitution) * velAlongNormal;
    //    j1 /= 1.0f / circleOneRigidBody->mMass;

    //    const Vec2 impulse = delta * j1;
    //    circleOneRigidBody->AddVelocity(impulse * (1.0f / circleOneRigidBody->GetMass()));
    //    if (abs(circleOneRigidBody->mVelocity.x) <= 0.02f)
    //    {
    //        circleOneRigidBody->SetVelocity(Vec2{ 0.0f, 0.0f });
    //        return;
    //    }
    //}
    //// todo check that the direction is correct, and it's not pushing both circles in the same direction.
    //{
    //    circle2->GetComponent<Transform>()->translation += delta * (overlap - SLOP) * PERCENT;

    //    const float velAlongNormal = Vec2::Dot(circleTwoRigidBody->mVelocity, delta);

    //    float j2 = -(1.0f + circleTwoRigidBody->mRestitution) * velAlongNormal;
    //    j2 /= 1.0f / circleTwoRigidBody->mMass;

    //    const Vec2 impulse = delta * j2;
    //    circleTwoRigidBody->AddVelocity(impulse * (1.0f / circleTwoRigidBody->GetMass()));
    //    if (abs(circleTwoRigidBody->mVelocity.x) <= 0.02f)
    //    {
    //        circleTwoRigidBody->SetVelocity(Vec2{ 0.0f, 0.0f });
    //    }
    //}
}

void CollisionHandler::ResolveCollisionBetweenBoxAndCircle(std::vector<CollisionInfo>& collisionInfos, WeakRef<GameObject> box,
                                                           WeakRef<GameObject> circle, WeakRef<CircleCollider> circleCollider)
{
    WeakRef<StaticBoxCollider> boxCollider    = box->GetComponent<StaticBoxCollider>();
    WeakRef<RigidBody>         rigidBody      = circle->GetComponent<RigidBody>();

    const Vec2  boxHalfExtents = boxCollider->GetHalfExtents();
    const float circleRadius   = circleCollider->GetRadius();

    const Vec2 boxCenter    = box->GetComponent<Transform>()->translation;
    const Vec2 circleCenter = circle->GetComponent<Transform>()->translation;

    const Vec2 closestPointOnBox
	{
        std::clamp(circleCenter.x, boxCenter.x - boxHalfExtents.x, boxCenter.x + boxHalfExtents.x),
        std::clamp(circleCenter.y, boxCenter.y - boxHalfExtents.y, boxCenter.y + boxHalfExtents.y)
    };

    Vec2 delta = circleCenter - closestPointOnBox;
    const float distanceSquared = Vec2::Dot(delta, delta);

    if (distanceSquared > circleRadius * circleRadius)
    {
        return;
    }

    // It is possible for the force from physics to push the object such that the circle centre is the closest point on the box.
    if (distanceSquared == 0.0f)
    {
        // If the force of the physics step puts the circle on the box, find out what its previous position was approximately, and use that instead.
        const Vec2 velocity = rigidBody->mVelocity;

        delta = circleCenter - velocity - closestPointOnBox;
        delta.Normalize();
    }
    else
    {
        delta /= sqrt(distanceSquared);
    }

    const float overlap = circleRadius - sqrt(distanceSquared);

    // Correct the position slightly to prevent jittering.
    constexpr float PERCENT = 0.8f;
    constexpr float SLOP    = 0.01f;

    circle->GetComponent<Transform>()->translation += delta * (overlap - SLOP) * PERCENT;

    const float velAlongNormal = Vec2::Dot(rigidBody->mVelocity, delta);

    if (velAlongNormal > 0.0f)
    {
        return;
    }

    float j = -(1.0f + rigidBody->mRestitution) * velAlongNormal;
    j /= 1.0f / rigidBody->mMass;

    const Vec2 impulse = delta * j;
    rigidBody->AddVelocity(impulse * (1.0f / rigidBody->GetMass()));

    collisionInfos.emplace_back(circle, box, closestPointOnBox);

    if (abs(rigidBody->mVelocity.x) <= 0.02f)
    {
        rigidBody->SetVelocity(Vec2{ 0.0f, 0.0f });
        return;
    }

    constexpr float mu = 0.6f;
    const float forceNormalMag = (rigidBody->mGravity * rigidBody->mMass).Magnitude();

    Vec2 movementDirection = rigidBody->mVelocity.x >= 0.0f ? Vec2{ 1.0f, 0.0f } : Vec2{ -1.0f, 0.0f };
    const Vec2 b = movementDirection * mu * forceNormalMag * -1.0f;
    rigidBody->AddForce(b);
}

} // Namespace d2e.