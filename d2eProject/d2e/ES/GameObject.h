#pragma once

namespace d2e
{

class IComponent;

class GameObject
{
public:
    GameObject()  = default;
    ~GameObject(); // todo delete components or change to be unique ptrs.

    template <typename Component>
    [[nodiscard]] WeakRef<Component> AddComponent();
private:
    std::vector<IComponent*> mComponents;
};


template <typename Component>
WeakRef<Component> GameObject::AddComponent()
{
    mComponents.emplace_back(new Component());
    return WeakRef{ reinterpret_cast<Component*>(mComponents.back()) };
}

} // Namespace d2e.