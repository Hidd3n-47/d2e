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

void Scene::Render(const  WeakRef<sf::RenderWindow> window, const OrthoCamera& camera) const
{
    for (const GameObject* gameObject : mGameObjects)
    {
        gameObject->Render(window, camera);
    }
}

WeakRef<GameObject> Scene::CreateGameObject()
{
    const Ulid id;
    mGameObjects.emplace_back(new GameObject{ id, WeakRef{ this } });
    mGameObjectIdToIndex[id] = mGameObjects.size() - 1;
    return WeakRef{ mGameObjects.back() };
}

WeakRef<GameObject> Scene::GetGameObject(const Ulid id)
{
    if (!mGameObjectIdToIndex.contains(id))
    {
        DEBUG_WARN("Failed to obtain game object with ID: {}", (uint64_t)id);
        return {};
    }

    return WeakRef{ mGameObjects[mGameObjectIdToIndex[id]] };
}

void Scene::UpdateGameObjectId(const Ulid oldId, const Ulid newId)
{
    const auto it = mGameObjectIdToIndex.find(oldId);
    if (it == mGameObjectIdToIndex.end())
    {
        DEBUG_WARN("Failed to obtain game object with ID: {}", (uint64_t)oldId);
        return;
    }

    mGameObjectIdToIndex[newId] = it->second;
    mGameObjectIdToIndex.erase(oldId);
}

} // Namespace d2e.
