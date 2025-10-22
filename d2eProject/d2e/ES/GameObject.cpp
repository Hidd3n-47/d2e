#include "d2ePch.h"
#include "GameObject.h"

#include "ES/Scene.h"
#include "Core/Rtti.h"

#include "Components/Transform.h"

namespace d2e
{

GameObject::GameObject(const Ulid id, const WeakRef<Scene> parent)
    : mId(id)
    , mParent(parent)
{
    AddComponent<Transform>();
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
    for (IComponent* component : mComponents)
    {
        component->Update(dt);
    }
}

void GameObject::Render(const WeakRef<sf::RenderWindow> window) const
{
    for (IComponent* component : mComponents)
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

void GameObject::SetId(const Ulid id)
{
    mParent->UpdateGameObjectId(mId, id);
    mId = id;
}

} // Namespace d2e.
