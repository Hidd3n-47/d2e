#pragma once

#include "d2e/src/d2ePch.h"
#include "d2e/Es/IComponent.h"

namespace d2e
{

class Scene;
class Transform;

class GameObject
{
public:
    GameObject(const uint32_t id, const WeakRef<Scene> parent);
    ~GameObject();

    void Update(const float dt) const;
    void Render(const WeakRef<sf::RenderWindow> window) const;

    [[nodiscard]] inline WeakRef<Scene> GetScene() const { return mParent; }

    WeakRef<IComponent> AddComponent(const std::string& component);

    template <typename Component>
    WeakRef<Component> AddComponent();

    template <typename Component, typename... Args>
    WeakRef<Component> AddComponent(Args ...args);

    template <typename Component>
    void RemoveComponent();

    WeakRef<IComponent> GetComponent(const std::string& component);

    template <typename Component>
    [[nodiscard]] WeakRef<Component> GetComponent() const;

    [[nodiscard]] inline const std::vector<IComponent*>& GetComponents() const { return mComponents; }

    [[nodiscard]] inline uint32_t GetId() const { return mId; }
    inline void SetId(const uint32_t id) { mId = id; }
private:
    uint32_t mId;
    WeakRef<Scene> mParent;

    //std::unique_ptr<Transform>  mTransform;
    std::vector<IComponent*> mComponents;
    std::unordered_map<std::string, IComponent*> mNameToComponentMap;
};

template <typename Component>
inline WeakRef<Component> GameObject::AddComponent()
{
    mComponents.emplace_back(new Component());
    mComponents.back()->OnComponentAdded(WeakRef{ this });
    mNameToComponentMap[Component::GetNameStatic()] = mComponents.back();
    return WeakRef{ reinterpret_cast<Component*>(mComponents.back()) };
}

template <typename Component, typename... Args>
inline WeakRef<Component> GameObject::AddComponent(Args ...args)
{
    mComponents.emplace_back(new Component(std::forward<Args>(args)...));
    mComponents.back()->OnComponentAdded(WeakRef{ this });
    mNameToComponentMap[Component::GetNameStatic()] = mComponents.back();
    return WeakRef{ reinterpret_cast<Component*>(mComponents.back()) };
}

template <typename Component>
void GameObject::RemoveComponent()
{
    mNameToComponentMap.erase(Component::GetNameStatic());

    for (auto it{ mComponents.begin() }; it != mComponents.end(); ++it)
    {
        if (Component* castComponent = dynamic_cast<Component*>(*it); castComponent != nullptr)
        {
            castComponent->OnComponentRemoved();

            mComponents.erase(it);

            delete castComponent;
            return;
        }
    }
}

template <typename Component>
inline WeakRef<Component> GameObject::GetComponent() const
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
inline WeakRef<Transform> GameObject::GetComponent() const
{
    return WeakRef{ reinterpret_cast<Transform*>(mComponents[0]) };
}

} // Namespace d2e.