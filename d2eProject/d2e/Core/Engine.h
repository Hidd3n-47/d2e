#pragma once

#include "d2e/src/d2ePch.h"

#include "d2e/Core/Ulid.h"
#include "d2e/Core/OrthoCamera.h"

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
    static void Destroy();

    inline void ChangeActiveScene(Scene* scene) { mSceneToChangeTo = scene; };

    void ConnectClientToServer(const int ip1, const int ip2, const int ip3, const int ip4, const uint16_t port);

    void StartFrame();
    void EndFrame() const;

    void Input() const;
    void SceneUpdate() const;
    void Update() const;
    void PostUpdate();
    void Render() const;
    void ProcessNetworkPackets() const;

    inline void SetOnPlayerTwoJoinedCallback(const std::function<void()>& callback) { mOnPlayerTwoJoined = callback; }

    [[nodiscard]] inline const OrthoCamera&         GetCamera()         const { return mOrthoCamera; }
    [[nodiscard]] inline WeakRef<Scene>             GetActiveScene()    const { return WeakRef{ mActiveScene }; }
    [[nodiscard]] inline WeakRef<InputManager>      GetInputManager()   const { return WeakRef{ mInputManager.get() }; }
    [[nodiscard]] inline WeakRef<sf::RenderWindow>  GetWindow()         const { return WeakRef{ mWindow.get() }; }
    [[nodiscard]] inline WeakRef<d2eNet::Client>    GetClient()         const { return WeakRef{ mClient.get() }; }
    DEBUG([[nodiscard]] inline WeakRef<Log>         GetLog()            const { return WeakRef{ mLog.get() }; })

    [[nodiscard]] inline float GetDeltaTime() const { return mDeltaTime; }

    inline void CloseGame() { mRunning = false; }

    static constexpr uint32_t TARGET_FRAMES = 120;
    static constexpr std::chrono::duration<float> TARGET_FRAME_TIME{ 1.0f / TARGET_FRAMES };

    static constexpr Ulid PLAYER_ONE_ULID{ 1 };
    static constexpr Ulid PLAYER_TWO_ULID{ 2 };
    static constexpr Ulid PING_DISPLAY_ULID{ 3 };
    static constexpr Ulid BATTLE_TIMER_ULID{ 4 };
    static constexpr Ulid BULLET_POOL_STARTING_ULID{ 100 };

    const Vec2 DEFAULT_SCREEN_SIZE{ 1920.0f, 1080.0f };
    inline static sf::Font GAME_FONT{ "E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Fonts/Liquidism/Liquidism.ttf" };
private:
    static std::unique_ptr<Engine>      mInstance;

    bool mRunning = true;
    std::unique_ptr<sf::RenderWindow>   mWindow;
    std::unique_ptr<InputManager>       mInputManager;
    std::unique_ptr<d2eNet::Client>     mClient;
    DEBUG(std::unique_ptr<Log> mLog);

    OrthoCamera mOrthoCamera;

    Scene* mActiveScene     = nullptr;
    Scene* mSceneToChangeTo = nullptr;

    float mDeltaTime = TARGET_FRAME_TIME.count();
    std::chrono::time_point<std::chrono::steady_clock> mFrameStart;

    std::function<void()> mOnPlayerTwoJoined;
};

} // Namespace d2e.
