#pragma once

#include "d2e/ES/IComponent.h"

namespace d2e
{

class RigidBody : public IComponent
{
friend class PhysicsManager;
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;
    void OnComponentRemoved() override;

    inline void AddForce(const Vec2 force) { mFnet += force; }

    [[nodiscard]] inline float GetMass() const { return mMass; }
    inline void SetMass(const float mass) { mMass = mass; }
private:
    float mMass = 1.0f;

    Vec2 mVelocity;
    Vec2 mAcceleration;

    Vec2 mFnet;
};

} // Namespace d2e.