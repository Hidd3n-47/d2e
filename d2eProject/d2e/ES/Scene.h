#pragma once

namespace d2e
{

class GameObject;

class Scene
{
public:
    Scene() = default;
    ~Scene(); //todo delete game objects or make it unique ptr.

    void Update() const {}

    [[nodiscard]] WeakRef<GameObject> CreateGameObject();
private:
    std::vector<GameObject*> mGameObjects;
};

} // Namespace d2e.