#pragma once

#include <d2e/src/d2ePch.h>

namespace d2eNet
{
class Client;
}

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
    void RemoveScene(WeakRef<Scene>& scene);
    bool SetActiveScene(const WeakRef<Scene>& scene);

    void ConnectClientToServer(const int ip1, const int ip2, const int ip3, const int ip4, const uint16_t port);

    void Input();
    void SendPackets() const;
    void Update() const;
    void Render() const;
    void ReceivePackets();

    [[nodiscard]] inline Vec2                       GetWindowSize()     const { return mWindowSize; }
    [[nodiscard]] inline WeakRef<Scene>             GetActiveScene()    const { return WeakRef{ mActiveScene }; }
    [[nodiscard]] inline WeakRef<InputManager>      GetInputManager()   const { return WeakRef{ mInputManager.get() }; }
    [[nodiscard]] inline WeakRef<sf::RenderWindow>  GetWindow()         const { return WeakRef{ mWindow.get() }; }
    [[nodiscard]] inline WeakRef<d2eNet::Client>    GetClient()         const { return WeakRef{ mClient.get() }; }
    DEBUG([[nodiscard]] inline WeakRef<Log>         GetLog()            const { return WeakRef{ mLog.get() }; })

    inline void CloseGame() { mRunning = false; }

    static constexpr uint32_t TARGET_FRAMES = 120;
    static constexpr std::chrono::duration<float> TARGET_FRAME_TIME{ 1.0f / TARGET_FRAMES };
private:
    // todo look at making it a shared ptr and then using std::weak_ptr instead of WeakRef.
    static std::unique_ptr<Engine>      mInstance;

    bool mRunning = true;
    std::unique_ptr<sf::RenderWindow>   mWindow;
    std::unique_ptr<InputManager>       mInputManager;
    std::unique_ptr<d2eNet::Client>     mClient;
    DEBUG(std::unique_ptr<Log> mLog);

    // todo look at changing from raw pointer for scenes.
    std::vector<Scene*> mScenes;
    Scene*              mActiveScene;

    float mDeltaTime = TARGET_FRAME_TIME.count();

    Vec2 mWindowSize{ 1920.0f, 1080.0f };
};

} // Namespace d2e.
