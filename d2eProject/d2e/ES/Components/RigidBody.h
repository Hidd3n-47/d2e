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

    inline void AddForce(const Vec2 force) { mForce += force; }
    inline void AddVelocity(const Vec2 velocity) { mVelocity += velocity; }
    inline void SetGravity(const Vec2 gravity) { mGravity = gravity; }

    [[nodiscard]] inline float GetMass() const { return mMass; }
    [[nodiscard]] inline float GetRestitution() const { return mRestitution; }
    [[nodiscard]] inline Vec2 GetVelocity() const { return mVelocity; }
    [[nodiscard]] inline Vec2 GetFnet() const { return mForce + mGravity; }

    inline void SetMass(const float mass) { mMass = mass; }
    inline void SetRestitution(const float restitution) { mRestitution = restitution; }
private:
    float mMass = 1.0f;
    float mRestitution;

    Vec2 mVelocity;
    Vec2 mAcceleration;

    Vec2 mForce;

    Vec2 mGravity;
};

} // Namespace d2e.