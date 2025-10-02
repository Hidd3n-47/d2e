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

    for (const WeakRef<GameObject> b : mBoxColliders)
    {
        for (const WeakRef<GameObject> c : mCircleColliders)
        {
            ResolveCollisionBetweenBoxAndCircle(b, c);
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

void CollisionHandler::RemoveBox(const WeakRef<GameObject> box)
{
    for (auto it{ mBoxColliders.begin() }; it != mBoxColliders.end(); ++it)
    {
        if (*it == box)
        {
            mBoxColliders.erase(it);
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
    WeakRef<BoxCollider>    boxCollider = box->GetComponent<BoxCollider>();
    WeakRef<CircleCollider> circleCollider = circle->GetComponent<CircleCollider>();

    const Vec2 boxHalfExtents = boxCollider->GetHalfExtents();
    const float circleRadius = circleCollider->GetRadius();

    const Vec2 boxCenter = box->GetComponent<Transform>()->translation;
    const Vec2 circleCenter = circle->GetComponent<Transform>()->translation;

    const Vec2 delta = circleCenter - boxCenter;
    if (abs(delta.x) > boxHalfExtents.x + circleRadius || abs(delta.y) > boxHalfExtents.y + circleRadius)
        return;

    /*bc->OnCollisionEnter(box, circle);
    cc->OnCollisionEnter(circle, box);*/

    if (BetweenMinAndMaxInclusive(boxCenter.x - boxHalfExtents.x, circleCenter.x, boxCenter.x + boxHalfExtents.x))
    {
        const float overlap = abs(delta.y) - (boxHalfExtents.y + circleRadius);

        if (delta.y > 0)
        {
            HandleCollisionBetweenBoxAndCircle(box, circle, Vec2{ 0.0f, overlap });
        }
        else
        {
            HandleCollisionBetweenBoxAndCircle(box, circle, Vec2{ 0.0f, -overlap });
        }

        return;
    }

    if (BetweenMinAndMaxInclusive(boxCenter.y - boxHalfExtents.y, circleCenter.y, boxCenter.y + boxHalfExtents.y))
    {
        float overlap = abs(delta.x) - (boxHalfExtents.x + circleRadius);

        if (delta.x < 0)
        {
            HandleCollisionBetweenBoxAndCircle(box, circle, Vec2{ -overlap, 0.0f });
        }
        else
        {
            HandleCollisionBetweenBoxAndCircle(box, circle, Vec2{ overlap, 0.0f });
        }

        return;
    }

    Vec2 cornerPosition;
    const bool circleAboveBox  = circleCenter.y > boxCenter.y;
    const bool circleLeftOfBox = circleCenter.x < boxCenter.x;

    if (circleAboveBox)
    {
        cornerPosition = circleLeftOfBox
            ? boxCenter + Vec2{ -boxHalfExtents.x, boxHalfExtents.y }  // Top Left Corner.
            : boxCenter + Vec2{  boxHalfExtents.x, boxHalfExtents.y }; // Top Right Corner.
    }
    else
    {
        cornerPosition = circleLeftOfBox
            ? boxCenter + Vec2{ -boxHalfExtents.x, -boxHalfExtents.y }  // Bottom Left Corner.
            : boxCenter + Vec2{  boxHalfExtents.x, -boxHalfExtents.y }; // Bottom Right Corner.
    }

    Vec2 direction = circleCenter - cornerPosition;
    const float distance = direction.Magnitude();

    const float overlap = distance - circleRadius;

    if (overlap >= 0.0f)
    {
        return;
    }

    // Normalize the direction to ensure unit direction.
    // Note we divide by length here as its more efficient than calling Normalize since we already computed the sqrt length.
    // Additionally, we don't have to worry about when length == 0.0f, as overlap <= 0.0f, meaning we would return from above prior to reaching this.
    direction /= distance;

    HandleCollisionBetweenBoxAndCircle(box, circle, direction * overlap);
}

void CollisionHandler::HandleCollisionBetweenBoxAndCircle(WeakRef<GameObject> box, WeakRef<GameObject> circle, const Vec2 overlap)
{
    if (!box->GetComponent<BoxCollider>()->IsFixed())
    {
        const Vec2 halfOverlap = overlap * 0.5f;

        circle->GetComponent<Transform>()->translation  -= halfOverlap;
        box->GetComponent<Transform>()->translation     += halfOverlap;

        return;
    }

    circle->GetComponent<Transform>()->translation -= overlap;
}

} // Namespace d2e.