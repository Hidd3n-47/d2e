#pragma once

#include <memory>

#include <Log/Log.h>
#include <d2e/Core/Engine.h>
#include <d2e/Core/WeakRef.h>

#include "GameState.h"
#include "Scene/IGameScene.h"
#include "src/Defines.h"

namespace d2eGame
{

class GameScene;
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

    DEBUG([[nodiscard]] inline d2e::WeakRef<d2e::Log>     GetLog() const { return d2e::WeakRef{ mLog.get() }; })

    void ChangeState(const GameState newState);

    void JoinOnlineGame();
private:
    static std::unique_ptr<GameManager> mInstance;

    DEBUG(std::unique_ptr<d2e::Log> mLog);

    GameState   mGameState      = GameState::NONE;
    IGameScene* mCurrentScene   = nullptr;

    template <typename Scene>
    [[nodiscard]] bool SetScene();
};

template <typename Scene>
bool GameManager::SetScene()
{
    static_assert(std::is_base_of_v<IGameScene, Scene>);
    IGameScene* scene = new Scene();

    if (!d2e::Engine::Instance()->SetActiveScene(scene->GetScene()))
    {
        GAME_ERROR("Failed to set active scene.");
        delete scene;
        return false;
    }

    delete mCurrentScene;
    mCurrentScene = scene;

    mCurrentScene->InitGameScene();

    return true;
}

} // Namespace d2eGame.