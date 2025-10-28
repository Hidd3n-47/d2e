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

/**
 * @class Engine: The engine class is responsible for the game logic and the running of the game.
 * On the client side, the engine will be used to run the full game, including the full game loop - namely Run
 * On the server side, the whole engine does not run, instead the engine is just used to run the update logic (so that the server can be headless).
 */
class Engine
{
public:
    Engine()  = default;
    ~Engine() = default;
    /**
     * @brief Get the instance of the engine that is used for the lifetime of the application.
     * @return A weak reference to the instance of the engine.
     */
    [[nodiscard]] inline static WeakRef<Engine> Instance() { return WeakRef{ mInstance.get() }; }

    Engine(const Engine&)             = delete;
    Engine(Engine&&)                  = delete;
    Engine& operator=(Engine&&)       = delete;
    Engine& operator=(const Engine&)  = delete;

    /**
     * @brief Initialize the engine and its requirements.
     * @note This doesn't have to be called on the server as the server runs a headless version.
     */
    void Init();
    /**
     * @brief Run the core gameplay loop of the game.
     * @note This should only be run on the client.
     */
    void Run();
    /**
     * @brief Destroy and clean up used resources by the engine.
     */
    static void Destroy();

    /**
     * @brief Change the active scene to be a different scene.
     * @note This change is not instant and will only occur in the \c PostUpdate.
     * @param scene The scene that is becoming active.
     */
    inline void ChangeActiveScene(Scene* scene) { mSceneToChangeTo = scene; }

    /**
     * @brief Connect the client to the server.
     * @param ip1 The first part of the IP address that the client is connecting to  i.e. IP1. X  . X  . X
     * @param ip2 The second part of the IP address that the client is connecting to i.e. X  . IP2. X  . X
     * @param ip3 The third part of the IP address that the client is connecting to  i.e. X  . X  . IP3. X
     * @param ip4 The forth part of the IP address that the client is connecting to  i.e. X  . X  . X  . IP4
     * @param port The port the connection runs through.
     */
    void ConnectClientToServer(const int ip1, const int ip2, const int ip3, const int ip4, const uint16_t port);

    /**
     * @brief Start the frame by capturing the time that the frame started.
     * @note This should be used on the client and server side to ensure that both are capped at the engine frame rate.
     */
    void StartFrame();
    /**
     * @brief End the frame by capturing the time the frame ended and computing the delta time of the frame.
     * @note This should be used on the client and server side to ensure that both are capped at the engine frame rate.
     */
    void EndFrame() const;

    /**
     * @brief Handle input.
     * @note This is only on the client side.
     */
    void Input() const;
    /**
     * @brief Update the scene.
     * @note Scene Updates occur before \c GameObjects, and hence \c Components, update occur.
     */
    void SceneUpdate() const;
    /**
     * @brief Update the \c GameObjects and \c Components.
     */
    void Update() const;
    /**
     * @brief An update that is used to preform actions that affect other objects, example, changing a scene.
     * @note Since this is trying to do operations where lifetime matters, this needs to be done on server side as well.
     */
    void PostUpdate();
    /**
     * @brief Render loop for the client.
     */
    void Render() const;
    /**
     * @brief Process the packets for the client side.
     */
    void ProcessNetworkPackets() const;

    /**
     * @brief Set the callback that is used when a second player joins the client.
     * @param callback The callback that is called when a second player joins the client.
     */
    inline void SetOnPlayerTwoJoinedCallback(const std::function<void()>& callback)    { mOnPlayerTwoJoined    = callback; }
    /**
     * @brief Set the callback that is used when a player disconnects from the server.
     * @param callback The callback that is called when a player disconnects from the server.
     */
    inline void SetOnPlayerDisconnectedCallback(const std::function<void()>& callback) { mOnPlayerDisconnected = callback; }

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

    /*
     * Constant \c Ulid that should be used to ensure the instances share the same \c Ulid between server and client.
     */
    static constexpr Ulid PLAYER_ONE_ULID{ 1 };
    static constexpr Ulid PLAYER_TWO_ULID{ 2 };
    static constexpr Ulid PING_DISPLAY_ULID{ 3 };
    static constexpr Ulid BATTLE_TIMER_ULID{ 4 };
    static constexpr Ulid BULLET_POOL_STARTING_ULID{ 100 };

    const Vec2 DEFAULT_SCREEN_SIZE{ 1920.0f, 1080.0f };
    inline static sf::Font GAME_FONT{ "Assets/Fonts/Liquidism/Liquidism.ttf" };
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
    std::function<void()> mOnPlayerDisconnected;
};

} // Namespace d2e.
