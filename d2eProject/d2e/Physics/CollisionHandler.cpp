#include "d2ePch.h"
#include "CollisionHandler.h"

#include "Core/Engine.h"
#include "Es/GameObject.h"
#include "ES/Components/Movement.h"

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
}

void CollisionHandler::AddBox(WeakRef<GameObject> box)
{
    mBoxColliders.emplace_back(box);
}

void CollisionHandler::ResolveCollisionBetweenBoxes(WeakRef<GameObject> box1, WeakRef<GameObject> box2)
{
    WeakRef<BoxCollider> box1Collider = box1->GetComponent<BoxCollider>();
    const Vec2 box1HalfExtent = box1Collider->GetHalfExtents();
    const Vec2 box1Centre = box1->GetComponent<Transform>()->translation;

    WeakRef<BoxCollider> box2Collider = box2->GetComponent<BoxCollider>();
    const Vec2 box2HalfExtent = box2Collider->GetHalfExtents();
    const Vec2 box2Centre = box2->GetComponent<Transform>()->translation;

    const Vec2 delta = box2Centre - box1Centre;
    if (abs(delta.x) < box1HalfExtent.x + box2HalfExtent.x && abs(delta.y) < box1HalfExtent.y + box2HalfExtent.y)
    {
        // There is a collision.
        DEBUG_LOG("collision");
        /*float moveBox1 = box1Collider->IsFixed() ? 0.0f : box2Collider->IsFixed() ? 0.5f : 0.5f;
        float moveBox2 = box2Collider->IsFixed() ? 0.0f : box1Collider->IsFixed() ? 0.5f : 0.5f;

        box1->GetComponent<Transform>()->translation -= delta * moveBox1;
        box2->GetComponent<Transform>()->translation += delta * moveBox2;*/
    }

    const Vec2 minDistance(box1HalfExtent.x + box2HalfExtent.x, box1HalfExtent.y + box2HalfExtent.y);

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

} // Namespace d2e.