#pragma once

#include <d2e/src/d2ePch.h>

namespace sf
{
class RenderWindow;
}

namespace d2e
{

class Scene;
class InputManager;

class Engine
{
public:
    Engine()  = default;
    ~Engine() = default;

    [[nodiscard]] inline static WeakRef<Engine> Instance() { return WeakRef{ mInstance.get() }; }

    Engine(const Engine&)             = delete;
    Engine(Engine&&)                  = delete;
    Engine& operator=(Engine&&)       = delete;
    Engine& operator=(const Engine&)  = delete;

    void Init();
    void Run();
    void Destroy();

    [[nodiscard]] WeakRef<Scene> CreateScene();
    bool SetActiveScene(const WeakRef<Scene>&scene);

    static constexpr uint32_t TARGET_FRAMES  = 120;
    static constexpr float TARGET_FRAME_TIME = 1.0f / TARGET_FRAMES;
private:
    // todo look at making it a shared ptr and then using std::weak_ptr instead of WeakRef.
    static std::unique_ptr<Engine> mInstance;

    std::unique_ptr<sf::RenderWindow>   mWindow;
    std::unique_ptr<InputManager>       mInputManager;

    std::vector<Scene*> mScenes;
    Scene* mActiveScene;

    float mDeltaTime = TARGET_FRAME_TIME;
    sf::Clock mFrameClock;

    void Update();
    void Render() const;
};

} // Namespace d2e.
