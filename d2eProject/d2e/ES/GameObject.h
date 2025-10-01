#pragma once

#include <d2e/src/d2ePch.h>

#include "d2e/Es/Components/Transform.h"

namespace d2e
{

class IComponent;
class Scene;

class GameObject
{
public:
    GameObject(const WeakRef<Scene> parent);
    ~GameObject(); // todo delete components or change to be unique ptrs.

    void Update(const float dt) const;
    void Render(const WeakRef<sf::RenderWindow> window) const;

    [[nodiscard]] inline WeakRef<Scene> GetScene() const { return mParent; }

    template <typename Component>
    WeakRef<Component> AddComponent();

    template <typename Component>
    [[nodiscard]] WeakRef<Component> GetComponent();
private:
    WeakRef<Scene> mParent;

    std::unique_ptr<Transform>  mTransform;
    std::vector<IComponent*>    mComponents;
};

template <typename Component>
inline WeakRef<Component> GameObject::AddComponent()
{
    mComponents.emplace_back(new Component());
    mComponents.back()->OnComponentAdded(WeakRef{ this });
    return WeakRef{ reinterpret_cast<Component*>(mComponents.back()) };
}

template <typename Component>
inline WeakRef<Component> GameObject::GetComponent()
{
    for (auto* comp : mComponents)
    {
        if (Component* castComponent = dynamic_cast<Component*>(comp); castComponent != nullptr)
        {
            return WeakRef{ castComponent };
        }
    }

    return WeakRef<Component>{};
}

template <>
inline WeakRef<Transform> GameObject::GetComponent()
{
    return WeakRef{ mTransform.get() };
}

} // Namespace d2e.