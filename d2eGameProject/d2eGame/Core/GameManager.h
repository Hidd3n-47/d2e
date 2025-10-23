#pragma once

#include <memory>

#include <Log/Log.h>
#include <d2e/Core/Engine.h>
#include <d2e/Core/WeakRef.h>

#include "Scene/GameScene.h"
#include "ApplicationState.h"

namespace d2eGame
{

class MainMenuScene;

class GameManager
{
public:
    GameManager();
    ~GameManager();

    GameManager(const GameManager&)             = delete;
    GameManager(GameManager&&)                  = delete;
    GameManager& operator=(GameManager&&)       = delete;
    GameManager& operator=(const GameManager&)  = delete;

    [[nodiscard]] inline static d2e::WeakRef<GameManager> Instance() { return d2e::WeakRef{ mInstance.get() }; }

    void Init();

    DEBUG([[nodiscard]] inline d2e::WeakRef<d2e::Log> GetLog() const { return d2e::WeakRef{ mLog.get() }; })

    void ChangeState(const ApplicationState newState);

    void JoinOnlineGame();
    void PlayerTwoJoined() const;
private:
    static std::unique_ptr<GameManager> mInstance;

    DEBUG(std::unique_ptr<d2e::Log> mLog);

    ApplicationState mApplicationState = ApplicationState::NONE;
    d2e::Scene* mCurrentScene   = nullptr;

    template <typename Scene>
    void SetScene();
};

template <typename Scene>
void GameManager::SetScene()
{
    static_assert(std::is_base_of_v<d2e::Scene, Scene>);

    d2e::Scene* scene = new Scene();

    d2e::Engine::Instance()->ChangeActiveScene(scene);

    mCurrentScene = scene;
}

} // Namespace d2eGame.