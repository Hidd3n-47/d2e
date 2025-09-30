#pragma once

#include <memory>

#include <Log/Log.h>
#include <d2e/Core/Engine.h>
#include <d2e/Core/WeakRef.h>

#include "GameState.h"
#include "Scene/IGameScene.h"

namespace d2eGame
{

class GameScene;
class MainMenuScene;

class GameManager
{
public:
    GameManager() = default;
    inline ~GameManager() { delete mCurrentScene; }

    GameManager(const GameManager&)             = delete;
    GameManager(GameManager&&)                  = delete;
    GameManager& operator=(GameManager&&)       = delete;
    GameManager& operator=(const GameManager&)  = delete;

    inline void Init() { ChangeState(GameState::MAIN_MENU); }

    [[nodiscard]] static inline d2e::WeakRef<GameManager> Instance() { return d2e::WeakRef{ mInstance.get() }; }

    void ChangeState(const GameState newState);
private:
    static std::unique_ptr<GameManager> mInstance;

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
        d2e::Log::Error("Failed to set active scene.");
        delete scene;
        return false;
    }

    delete mCurrentScene;
    mCurrentScene = scene;
    return true;
}

} // Namespace d2eGame.