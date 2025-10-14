#pragma once

namespace d2e
{

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
protected:
    WeakRef<GameObject> mParent;
};

} // Namespace d2e.