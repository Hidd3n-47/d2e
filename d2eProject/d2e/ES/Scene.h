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
    ~Scene();

    void Update(const float dt) const;
    void Render(const WeakRef<sf::RenderWindow> window) const;

    [[nodiscard]] WeakRef<GameObject> CreateGameObject();

    [[nodiscard]] inline WeakRef<PhysicsManager>    GetPhysicsManager()     { return WeakRef{ &mPhysicsManager }; }
    [[nodiscard]] inline WeakRef<CollisionHandler>  GetCollisionHandler()   { return WeakRef{ &mCollisionHandler }; }
private:
    std::vector<GameObject*> mGameObjects;

    PhysicsManager   mPhysicsManager;
    CollisionHandler mCollisionHandler;
};

} // Namespace d2e.