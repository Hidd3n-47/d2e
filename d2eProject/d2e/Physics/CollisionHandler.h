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

    void RemoveBox(const WeakRef<GameObject> box);
    void RemoveCircle(const WeakRef<GameObject> circle);
private:
    std::vector<WeakRef<GameObject>> mBoxColliders;
    std::vector<WeakRef<GameObject>> mCircleColliders;

    static void ResolveCollisionBetweenBoxes(WeakRef<GameObject> box1, WeakRef<GameObject> box2);
    static void ResolveCollisionBetweenCircles(WeakRef<GameObject> circle1, WeakRef<GameObject> circle2);
    static void ResolveCollisionBetweenBoxAndCircle(WeakRef<GameObject> box, WeakRef<GameObject> circle);
    static void HandleCollisionBetweenBoxAndCircle(WeakRef<GameObject> box, WeakRef<GameObject> circle, const Vec2 overlap);

    static inline float Sign(const float value) { return value < 0.0f ? -1.0f : 1.0f; }
    static inline bool BetweenMinAndMaxInclusive(const float min, const float value, const float max) { return min <= value && value <= max; }
};

} // Namespace d2e.