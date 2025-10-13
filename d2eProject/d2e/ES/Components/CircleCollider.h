#pragma once

#include "d2e/Core/Rtti.h"
#include "d2e/ES/IComponent.h"

namespace d2e
{

struct CollisionInfo;

class CircleCollider : public IComponent
{
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;
    void OnComponentRemoved() override;

    void OnCollisionEnter(const CollisionInfo& info) const;

    void UpdateObjectsCollidedWith(const std::vector<CollisionInfo>& collisionInfos);

    [[nodiscard]] inline bool  GetCollidedLastFrame() const { return mCollidedLastFrame; }
    [[nodiscard]] inline float GetRadius() const { return mRadius; }

    inline void SetRadius(const float radius) { mRadius = radius; }
    inline void SetOnCollisionEnterCallback(const std::function<void(const CollisionInfo&)>& callback) { mOnCollisionEnterCallback = callback; }

    D2E_COMPONENT(CircleCollider)

#ifdef DEV_CONFIGURATION
public:
    void Render(WeakRef<sf::RenderWindow> window) override;
private:
    sf::CircleShape mDebugRender;
public:
#endif // DEV_CONFIGURATION.
private:
    float mRadius            = 10.0f;
    bool  mCollidedLastFrame = false;

    std::function<void(const CollisionInfo&)> mOnCollisionEnterCallback;
    std::set<WeakRef<GameObject>> mObjectsCollidingWith;
};

} // Namespace d2e.