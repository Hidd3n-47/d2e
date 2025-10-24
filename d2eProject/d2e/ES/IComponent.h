#pragma once

#include "d2e/Core/OrthoCamera.h"

namespace d2e
{

#define D2E_COMPONENT(X) \
    [[nodiscard]] inline std::string GetName() const override { return GetNameStatic(); } \
    [[nodiscard]] inline static std::string GetNameStatic() { return ##X; }

class GameObject;

class IComponent
{
public:
    virtual ~IComponent() = default;

    virtual void OnComponentAdded(const WeakRef<GameObject> parent) { mParent = parent; }
    virtual void OnComponentRemoved() { }

    virtual void Update(const float dt) { }
    virtual void Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera) { }

    [[nodiscard]] virtual std::string Serialize() const { return ""; }
    virtual void Deserialize(const std::string& string) { }

    [[nodiscard]] inline bool SyncValuesOnUpdate() const { return mSyncValuesOnUpdate; }
    inline void SetSyncValuesOnUpdate(const bool sync) { mSyncValuesOnUpdate = sync; }

    virtual std::string GetName() const { return "component"; }
protected:
    WeakRef<GameObject> mParent;
    bool mSyncValuesOnUpdate = false;
};

} // Namespace d2e.