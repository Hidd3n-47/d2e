#pragma once

namespace d2e
{

class GameObject;

class PhysicsManager
{
public:
    void Update() const;

    void AddBody(const WeakRef<GameObject> body);
    void RemoveBody(const WeakRef<GameObject> body);
private:
    std::vector<WeakRef<GameObject>> mBodies;
};

} // Namespace d2e.