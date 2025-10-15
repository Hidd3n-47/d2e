#pragma once

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
    virtual void Render(WeakRef<sf::RenderWindow> window) { }

    [[nodiscard]] virtual std::string Serialize() const { return ""; }
    virtual void Deserialize(const std::string& string) { }

    virtual std::string GetName() const { return "component"; }
protected:
    WeakRef<GameObject> mParent;
};

} // Namespace d2e.