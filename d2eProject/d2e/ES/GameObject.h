#pragma once

#include "d2e/src/d2ePch.h"

#include "d2e/Core/Ulid.h"
#include "d2e/Es/IComponent.h"
#include "d2e/Core/OrthoCamera.h"

namespace d2e
{

class Scene;
class Transform;

/**
 * @class GameObject: A class representing the abstract idea of a 'Game Object'. A game object is just a collection of \c Components.
 */
class GameObject
{
public:
    GameObject(const Ulid id, const WeakRef<Scene> parent);
    ~GameObject();

    void Update(const float dt) const;
    void Render(const WeakRef<sf::RenderWindow> window, const OrthoCamera& camera) const;

    [[nodiscard]] inline WeakRef<Scene> GetScene() const { return mParent; }

    /**
     * @brief Add a component by the component type.
     * @param component The name of the component type.
     * @return A reference to the component that was added.
     */
    WeakRef<IComponent> AddComponent(const std::string& component);

    /**
     * @brief Add a component.
     * @tparam Component The component type being added.
     * @return A reference to the component that was added.
     */
    template <typename Component>
    WeakRef<Component> AddComponent();

    /**
     * @brief Add A component and pass in the arguments into the component constructor.
     * @tparam Component The component type being added.
     * @tparam Args The arguments types being forwarded into the components constructor.
     * @param args The arguments being forwarded into the components constructor.
     * @return
     */
    template <typename Component, typename... Args>
    WeakRef<Component> AddComponent(Args ...args);

    /**
     * @brief Remove a component.
     * @tparam Component The component type that is being removed.
     */
    template <typename Component>
    void RemoveComponent();

    /**
     * @brief Get a component based off its type name.
     * @param component The name of the component that is being requested.
     * @return A reference to the component. This reference might be invalid if not found
     * @see \c WeakRef.
     */
    WeakRef<IComponent> GetComponent(const std::string& component);
    /**
     * @brief Get a component based of its type.
     * @tparam Component The type of the components that is being requested for.
     * @return A reference to the component. This reference might be invalid if not found
     * @see \c WeakRef.
     */
    template <typename Component>
    [[nodiscard]] WeakRef<Component> GetComponent() const;

    /**
     * @brief Get the vector of components that are attached to the game object.
     * @return The vector of components attached to the game object.
     */
    [[nodiscard]] inline const std::vector<IComponent*>& GetComponents() const { return mComponents; }

    [[nodiscard]] inline Ulid GetId() const { return mId; }
    void SetId(const Ulid id);
private:
    Ulid   mId;
    WeakRef<Scene> mParent;

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