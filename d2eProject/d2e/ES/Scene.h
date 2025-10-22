#pragma once

#include "d2e/Es/GameObject.h"
#include "d2e/Physics/PhysicsManager.h"
#include "d2e/Physics/CollisionHandler.h"

namespace d2e
{

class Scene
{
public:
    Scene() = default;
    virtual ~Scene();

    inline virtual void InitScene() { mScenePaused = true; }
    inline virtual void SceneUpdate() const { }

    void Update(const float dt) const;
    void Render(const WeakRef<sf::RenderWindow> window) const;

    [[nodiscard]] WeakRef<GameObject> CreateGameObject();

    [[nodiscard]] WeakRef<GameObject> GetGameObject(const Ulid id);

    void UpdateGameObjectId(const Ulid oldId, const Ulid newId);

    [[nodiscard]] inline WeakRef<PhysicsManager>    GetPhysicsManager()     { return WeakRef{ &mPhysicsManager }; }
    [[nodiscard]] inline WeakRef<CollisionHandler>  GetCollisionHandler()   { return WeakRef{ &mCollisionHandler }; }

    [[nodiscard]] inline bool IsScenePaused() const { return mScenePaused; }
    inline void PauseScene(bool paused = true)      { mScenePaused = paused; }
protected:
    bool mScenePaused = false;
private:
    std::vector<GameObject*> mGameObjects;

    std::unordered_map<Ulid, size_t> mGameObjectIdToIndex;

    PhysicsManager   mPhysicsManager;
    CollisionHandler mCollisionHandler;
};

} // Namespace d2e.