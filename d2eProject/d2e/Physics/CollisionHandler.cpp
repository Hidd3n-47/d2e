#include "d2ePch.h"
#include "CollisionHandler.h"

#include "ES/GameObject.h"
#include "ES/Components/BoxCollider.h"
#include "ES/Components/CircleCollider.h"

namespace d2e
{

void CollisionHandler::Update() const
{
    for (const WeakRef<GameObject> b : mBoxColliders)
    {
        for (const WeakRef<GameObject> b2 : mBoxColliders)
        {
            if (b == b2)
            {
                continue;
            }

            ResolveCollisionBetweenBoxes(b, b2);
        }
    }

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
}

void CollisionHandler::AddBox(const WeakRef<GameObject> box)
{
    mBoxColliders.emplace_back(box);
}

void CollisionHandler::AddCircle(const WeakRef<GameObject> circle)
{
    mCircleColliders.emplace_back(circle);
}

void CollisionHandler::ResolveCollisionBetweenBoxes(WeakRef<GameObject> box1, WeakRef<GameObject> box2)
{
    WeakRef<BoxCollider> box1Collider = box1->GetComponent<BoxCollider>();
    const Vec2 box1HalfExtent   = box1Collider->GetHalfExtents();
    const Vec2 box1Centre       = box1->GetComponent<Transform>()->translation;

    WeakRef<BoxCollider> box2Collider = box2->GetComponent<BoxCollider>();
    const Vec2 box2HalfExtent   = box2Collider->GetHalfExtents();
    const Vec2 box2Centre       = box2->GetComponent<Transform>()->translation;

    const Vec2 delta = box2Centre - box1Centre;

    const Vec2 minDistance{ box1HalfExtent.x + box2HalfExtent.x, box1HalfExtent.y + box2HalfExtent.y };

    if (abs(delta.x) >= minDistance.x || abs(delta.y) >= minDistance.y)
    {
        return;
    }

    const float overlapX = minDistance.x - abs(delta.x);
    const float overlapY = minDistance.y - abs(delta.y);

    if (overlapX < overlapY)
    {
        const float halfOverlap = -0.5f * Sign(delta.x) * overlapX;
        if (!box1Collider->IsFixed())
        {
            box1->GetComponent<Transform>()->translation.x += halfOverlap;
        }
        else
        {
            box2->GetComponent<Transform>()->translation.x -= halfOverlap;
        }

        if (!box2Collider->IsFixed())
        {
            box2->GetComponent<Transform>()->translation.x -= halfOverlap;
        }
        else
        {
            box1->GetComponent<Transform>()->translation.x += halfOverlap;
        }
    }
    else
    {
        const float halfOverlap = -0.5f * Sign(delta.y) * overlapY;
        if (!box1Collider->IsFixed())
        {
            box1->GetComponent<Transform>()->translation.y += halfOverlap;
        }
        else
        {
            box2->GetComponent<Transform>()->translation.y -= halfOverlap;
        }

        if (!box2Collider->IsFixed())
        {
            box2->GetComponent<Transform>()->translation.y -= halfOverlap;
        }
        else
        {
            box1->GetComponent<Transform>()->translation.y += halfOverlap;
        }
    }
}

void CollisionHandler::ResolveCollisionBetweenCircles(WeakRef<GameObject> circle1, WeakRef<GameObject> circle2)
{
    WeakRef<CircleCollider> circleCollider = circle1->GetComponent<CircleCollider>();
    const float circle1Radius   = circleCollider->GetRaidus();
    const Vec2  circle1Centre   = circle1->GetComponent<Transform>()->translation;

    WeakRef<CircleCollider> circle2Collider = circle2->GetComponent<CircleCollider>();
    const float circle2Radius   = circle2Collider->GetRaidus();
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

} // Namespace d2e.