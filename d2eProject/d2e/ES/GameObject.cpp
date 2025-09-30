#include "d2ePch.h"
#include "GameObject.h"

#include "Components/Transform.h"

namespace d2e
{
GameObject::GameObject()
    : mTransform{ std::make_unique<Transform>() }
{
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

} // Namespace d2e.
