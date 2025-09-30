#pragma once

#include "d2e/Es/GameObject.h"

namespace d2e
{

class Scene
{
public:
    Scene() = default;
    ~Scene(); //todo delete game objects or make it unique ptr.

    void OnSceneCreated() { }

    void Update(const float dt) const;
    void Render(const WeakRef<sf::RenderWindow> window) const;

    [[nodiscard]] WeakRef<GameObject> CreateGameObject();
private:
    std::vector<GameObject*> mGameObjects;
};

} // Namespace d2e.