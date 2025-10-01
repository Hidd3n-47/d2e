#include "d2ePch.h"
#include "Scene.h"

namespace d2e
{

void Scene::Update(const float dt) const
{
    for (const auto* gameObject : mGameObjects)
    {
        gameObject->Update(dt);
    }

    mCollisionHandler.Update();
}

void Scene::Render(const  WeakRef<sf::RenderWindow> window) const
{
    for (const auto* gameObject : mGameObjects)
    {
        gameObject->Render(window);
    }
}

WeakRef<GameObject> Scene::CreateGameObject()
{
    mGameObjects.emplace_back(new GameObject(WeakRef{ this }));
    return WeakRef{ mGameObjects.back() };
}

} // Namespace d2e.
