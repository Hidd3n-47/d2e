#pragma once

#include "d2e/Core/Rtti.h"
#include "d2e/ES/IComponent.h"

namespace d2e
{

class RigidBody : public IComponent
{
friend class PhysicsManager;
friend class CollisionHandler;
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;
    void OnComponentRemoved() override;

    inline void AddForce(const Vec2 force)       { mForce += force; }
    inline void AddVelocity(const Vec2 velocity) { mVelocity += velocity; EnsureVelocityIsUnderMaxSpeed(); }

    [[nodiscard]] inline float GetMass()        const { return mMass; }
    [[nodiscard]] inline float GetRestitution() const { return mRestitution; }
    [[nodiscard]] inline float GetMaxSpeed()    const { return mMaxSpeed; }
    [[nodiscard]] inline Vec2  GetVelocity()    const { return mVelocity; }
    [[nodiscard]] inline Vec2  GetGravity()     const { return mGravity; }
    [[nodiscard]] inline Vec2  GetFnet()        const { return mForce + mGravity; }

    inline void SetMass(const float mass)               { mMass = mass; }
    inline void SetMaxSpeed(const float maxSpeed)       { mMaxSpeed = maxSpeed; }
    inline void SetRestitution(const float restitution) { mRestitution = restitution; }
    inline void SetVelocity(const Vec2 velocity)        { mVelocity = velocity; EnsureVelocityIsUnderMaxSpeed(); }
    inline void SetGravity(const Vec2 gravity)          { mGravity = gravity; }

    D2E_COMPONENT(RigidBody)
private:
    float mMass        = 1.0f;
    float mRestitution = 0.1f;
    float mMaxSpeed    = 500.0f;

    Vec2 mVelocity;
    Vec2 mAcceleration;

    Vec2 mGravity;

    Vec2 mForce;

    void EnsureVelocityIsUnderMaxSpeed();
};

} // Namespace d2e.