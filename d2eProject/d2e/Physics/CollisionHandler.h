#pragma once

#include "d2e/ES/Components/BoxCollider.h"

namespace d2e
{

class CollisionHandler
{
public:
    void Update() const;

    void AddBox(WeakRef<GameObject> box);
private:
    std::vector<WeakRef<GameObject>> mBoxColliders;

    static void ResolveCollisionBetweenBoxes(WeakRef<GameObject> box1, WeakRef<GameObject> box2);

    static inline float Sign(const float value) { return value < 0.0f ? -1.0f : 1.0f; }
};

} // Namespace d2e.