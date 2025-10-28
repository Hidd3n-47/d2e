#pragma once

#include "d2e/Core/OrthoCamera.h"

namespace d2e
{

#define D2E_COMPONENT(X) \
    [[nodiscard]] inline std::string GetName() const override { return GetNameStatic(); } \
    [[nodiscard]] inline static std::string GetNameStatic() { return ##X; }

class GameObject;

/**
 * @class IComponent: A class that gives components its bare functionality.
 * @note All components should inherit from this.<br/>
 *       All components should include the macro D2E_COMPONENT(X), where X=Component's name/type.<br/>
 *       All components should then be added into the Component Map in Rtti.h
 */
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

    /**
     * @brief Get if the component's values are being synchronised with the server every frame.
     * @return If the component's values are being synchronised with the server every frame.
     */
    [[nodiscard]] inline bool SyncValuesOnUpdate() const { return mSyncValuesOnUpdate; }
    inline void SetSyncValuesOnUpdate(const bool sync) { mSyncValuesOnUpdate = sync; }

    virtual std::string GetName() const { return "component"; }
protected:
    WeakRef<GameObject> mParent;
    bool mSyncValuesOnUpdate = false;
};

} // Namespace d2e.