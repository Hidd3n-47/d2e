#pragma once

namespace d2e
{

class GameObject;

class CollisionHandler
{
public:
    void Update() const;

    void AddStaticBox(const WeakRef<GameObject> box);
    void AddCircle(const WeakRef<GameObject> circle);

    void RemoveStaticBox(const WeakRef<GameObject> box);
    void RemoveCircle(const WeakRef<GameObject> circle);
private:
    std::vector<WeakRef<GameObject>> mStaticBoxColliders;
    std::vector<WeakRef<GameObject>> mCircleColliders;

    static void ResolveCollisionBetweenCircles(WeakRef<GameObject> circle1, WeakRef<GameObject> circle2);
    static void ResolveCollisionBetweenBoxAndCircle(WeakRef<GameObject> box, WeakRef<GameObject> circle);
};

} // Namespace d2e.