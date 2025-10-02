#include "d2ePch.h"
#include "Scene.h"

namespace d2e
{

Scene::~Scene()
{
    for (const GameObject* go : mGameObjects)
    {
        delete go;
    }
}

void Scene::Update(const float dt) const
{
    for (const GameObject* gameObject : mGameObjects)
    {
        gameObject->Update(dt);
    }

    mCollisionHandler.Update();
    mPhysicsManager.Update();
}

void Scene::Render(const  WeakRef<sf::RenderWindow> window) const
{
    for (const GameObject* gameObject : mGameObjects)
    {
        gameObject->Render(window);
    }
}

WeakRef<GameObject> Scene::CreateGameObject()
{
    mGameObjects.emplace_back(new GameObject{ WeakRef{ this } });
    return WeakRef{ mGameObjects.back() };
}

} // Namespace d2e.
