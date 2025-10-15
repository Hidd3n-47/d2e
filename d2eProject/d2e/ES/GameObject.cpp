#include "d2ePch.h"
#include "GameObject.h"

#include <d2e/Core/Rtti.h>

#include "Components/Transform.h"

namespace d2e
{
GameObject::GameObject(const uint32_t id, const WeakRef<Scene> parent)
    : mId(id)
    , mParent(parent)
    //, mTransform{ std::make_unique<Transform>() }
{
    AddComponent<Transform>();
    //mNameToComponentMap[Transform::GetNameStatic()] = dynamic_cast<IComponent*>(mTransform.get());
    // Empty.
}

GameObject::~GameObject()
{
    for (const auto comp : mComponents)
    {
        delete comp;
    }
}

void GameObject::Update(const float dt) const
{
    for (auto* component : mComponents)
    {
        component->Update(dt);
    }
}

void GameObject::Render(const WeakRef<sf::RenderWindow> window) const
{
    for (auto* component : mComponents)
    {
        component->Render(window);
    }
}

WeakRef<IComponent> GameObject::AddComponent(const std::string& component)
{
    const auto& map = GetComponentMap();

    const auto it = map.find(component);
    if (it == map.end())
    {
        DEBUG_BREAK();
        return {};
    }

    return it->second(WeakRef{ this });
}

WeakRef<IComponent> GameObject::GetComponent(const std::string& component)
{
    return WeakRef{ mNameToComponentMap[component] };
}

} // Namespace d2e.
