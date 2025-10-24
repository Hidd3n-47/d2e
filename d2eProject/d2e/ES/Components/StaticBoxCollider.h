#pragma once

#include "d2e/ES/IComponent.h"
#include "d2e/Core/SerializationUtils.h"

namespace d2e
{

class StaticBoxCollider : public IComponent
{
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;
    void OnComponentRemoved() override;

    [[nodiscard]] inline Vec2 GetHalfExtents() const { return mHalfExtents; }

    inline void SetHalfExtents(const Vec2 halfExtents) { mHalfExtents = halfExtents; }

    [[nodiscard]] inline std::string Serialize() const override { return SerializeUtils::Serialize(mSyncValuesOnUpdate) + SerializeUtils::Serialize(mHalfExtents); }
    inline void Deserialize(const std::string& string) override { SerializeUtils::Deserialize(mSyncValuesOnUpdate, std::string{ string[0] }); SerializeUtils::Deserialize(mHalfExtents, string.substr(1)); }

    D2E_COMPONENT("StaticBoxCollider")

#ifdef DEV_CONFIGURATION
public:
    void Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera) override;
private:
    sf::RectangleShape mDebugRender;
public:
#endif // DEV_CONFIGURATION.
private:
    Vec2 mHalfExtents{ 0.5f, 0.5f };
};

} // Namespace d2e.