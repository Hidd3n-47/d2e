#pragma once

#include "d2e/ES/IComponent.h"
#include "d2e/Core/SerializationUtils.h"

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

    [[nodiscard]] inline std::string Serialize() const override { return SerializeUtils::Serialize(mCollidedLastFrame) + SerializeUtils::Serialize(mRadius); }
    inline void Deserialize(const std::string& string) override { SerializeUtils::Deserialize(mCollidedLastFrame, std::string{ string[0] }); SerializeUtils::Deserialize(mRadius, string.substr(1)); }

    D2E_COMPONENT("CircleCollider")

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