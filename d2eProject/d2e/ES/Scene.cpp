#include "d2ePch.h"
#include "Scene.h"

#include "Core/Engine.h"

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
    mPhysicsManager.Update(dt);
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
    mGameObjects.emplace_back(new GameObject{ ++mGameObjectId, WeakRef{ this } });
    mGameObjectIdToIndex[mGameObjectId] = mGameObjects.size() - 1;
    return WeakRef{ mGameObjects.back() };
}

WeakRef<GameObject> Scene::GetGameObject(const uint32_t id)
{
    if (!mGameObjectIdToIndex.contains(id))
    {
        DEBUG_WARN("Failed to obtain game object with ID: {}", id);
        return {};
    }

    return WeakRef{ mGameObjects[mGameObjectIdToIndex[id]] };
}
} // Namespace d2e.
