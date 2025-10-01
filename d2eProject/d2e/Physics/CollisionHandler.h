#pragma once

namespace d2e
{

class GameObject;

class CollisionHandler
{
public:
    void Update() const;

    void AddBox(const WeakRef<GameObject> box);
    void AddCircle(const WeakRef<GameObject> circle);
private:
    std::vector<WeakRef<GameObject>> mBoxColliders;
    std::vector<WeakRef<GameObject>> mCircleColliders;

    static void ResolveCollisionBetweenBoxes(WeakRef<GameObject> box1, WeakRef<GameObject> box2);
    static void ResolveCollisionBetweenCircles(WeakRef<GameObject> circle1, WeakRef<GameObject> circle2);

    static inline float Sign(const float value) { return value < 0.0f ? -1.0f : 1.0f; }
};

} // Namespace d2e.