#include "d2ePch.h"
#include "CollisionHandler.h"

#include "Core/Engine.h"
#include "ES/GameObject.h"
#include "ES/Components/StaticBoxCollider.h"
#include "ES/Components/CircleCollider.h"
#include "ES/Components/RigidBody.h"

namespace d2e
{

void CollisionHandler::Update() const
{
    for (const WeakRef<GameObject> c : mCircleColliders)
    {
        for (const WeakRef<GameObject> c2 : mCircleColliders)
        {
            if (c == c2)
            {
                continue;
            }

            ResolveCollisionBetweenCircles(c, c2);
        }
    }

    for (const WeakRef<GameObject> b : mStaticBoxColliders)
    {
        for (const WeakRef<GameObject> c : mCircleColliders)
        {
            ResolveCollisionBetweenBoxAndCircle(b, c);
        }
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

void CollisionHandler::ResolveCollisionBetweenCircles(WeakRef<GameObject> circle1, WeakRef<GameObject> circle2)
{
    WeakRef<CircleCollider> circleCollider = circle1->GetComponent<CircleCollider>();
    const float circle1Radius   = circleCollider->GetRadius();
    const Vec2  circle1Centre   = circle1->GetComponent<Transform>()->translation;

    WeakRef<CircleCollider> circle2Collider = circle2->GetComponent<CircleCollider>();
    const float circle2Radius   = circle2Collider->GetRadius();
    const Vec2  circle2Centre   = circle2->GetComponent<Transform>()->translation;

    Vec2 delta = circle2Centre - circle1Centre;
    const float length = delta.Magnitude();

    const float minDistance = circle1Radius + circle2Radius;

    if (length >= minDistance)
    {
        return;
    }

    // Normalize delta so we only take into account the direction between the centres.
    // Note we divide by length here as its more efficient than calling Normalize since we already computed the sqrt length.
    // Additionally, we don't have to worry about when length == 0.0f, as minDistance == 0.0f, meaning we would return from above prior to reaching this.
    delta /= length;

    const float halfOverlap = (minDistance - length) * 0.5f;

    circle1->GetComponent<Transform>()->translation +=  delta * halfOverlap * -1.0f;
    circle2->GetComponent<Transform>()->translation +=  delta * halfOverlap;
}

void CollisionHandler::ResolveCollisionBetweenBoxAndCircle(WeakRef<GameObject> box, WeakRef<GameObject> circle)
{
    WeakRef<StaticBoxCollider> boxCollider    = box->GetComponent<StaticBoxCollider>();
    WeakRef<CircleCollider>    circleCollider = circle->GetComponent<CircleCollider>();
    WeakRef<RigidBody>         rigidBody      = circle->GetComponent<RigidBody>();

    const Vec2 boxHalfExtents = boxCollider->GetHalfExtents();
    const float circleRadius = circleCollider->GetRadius();

    const Vec2 boxCenter = box->GetComponent<Transform>()->translation;
    const Vec2 circleCenter = circle->GetComponent<Transform>()->translation;

    Vec2 closestPointOnBox{
        std::clamp(circleCenter.x, boxCenter.x - boxHalfExtents.x, boxCenter.x + boxHalfExtents.x),
        std::clamp(circleCenter.y, boxCenter.y - boxHalfExtents.y, boxCenter.y + boxHalfExtents.y)
    };

    Vec2 delta = circleCenter - closestPointOnBox;
    const float distanceSquared = Vec2::Dot(delta, delta);

    if (distanceSquared > circleRadius * circleRadius)
    {
        return;
    }


    delta /= sqrt(distanceSquared);

    const float overlap = circleRadius - sqrt(distanceSquared);

    // Correct the position slightly to prevent jittering.
    constexpr float PERCENT = 0.8f;
    constexpr float SLOP = 0.01f;

    const Vec2 correction = delta * std::max(overlap - SLOP, 0.0f) * PERCENT;

    circle->GetComponent<Transform>()->translation += correction;

    const float velAlongNormal = Vec2::Dot(rigidBody->GetVelocity(), delta);

    if (velAlongNormal > 0.0f)
    {
        return;
    }

    // If the velocity is moving away
    /*constexpr float EPSILON = 0.1f;
    if (fabs(velAlongNormal) < EPSILON)
    {
        rigidBody->AddVelocity(delta * -velAlongNormal);
        return;
    }*/

    float j = -(1.0f + rigidBody->GetRestitution()) * velAlongNormal;
    j /= 1.0f / rigidBody->GetMass();

    const Vec2 impulse = delta * j;
    rigidBody->AddVelocity(impulse * (1.0f / rigidBody->GetMass()));

    //constexpr float dragCoefficient = 0.01f;
    //const Vec2 drag = Vec2{ rigidBody->GetVelocity().x, 0.0f } * -dragCoefficient;
    //rigidBody->AddForce(drag);

    if (abs(rigidBody->GetVelocity().x) <= 0.02f) { rigidBody->SetVelocity(Vec2{ 0.0f, 0.0f }); return; }

    constexpr float mu = 0.6f;
    const float forceNormalMag = (rigidBody->GetGravity() * rigidBody->GetMass()).Magnitude();

    Vec2 movementDirection = rigidBody->GetVelocity().x >= 0.0f ? Vec2{ 1.0f, 0.0f } : Vec2{ -1.0f, 0.0f };
    const Vec2 b = movementDirection * mu * forceNormalMag * -1.0f;
    rigidBody->AddForce(b);

    //float frictionCoeff = 0.2f; // tune this
    //Vec2 friction = Vec2(rigidBody->GetVelocity().x, 0) * -frictionCoeff;
    //rigidBody->AddVelocity(friction);
}

} // Namespace d2e.