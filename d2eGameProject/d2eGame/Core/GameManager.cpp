#include "GameManager.h"

#include "src/Defines.h"
#include "Scene/GameScene.h"
#include "Scene/MainMenuScene.h"

namespace d2eGame
{

std::unique_ptr<GameManager> GameManager::mInstance = std::make_unique<GameManager>();

void GameManager::ChangeState(const GameState newState)
{
    if (mGameState == newState)
    {
        d2e::Log::Warn("Trying to change game state to the same state.");
        DEBUG_BREAK();
        return;
    }

    bool changedScene = false;
    switch (newState)
    {
    case GameState::MAIN_MENU:
        changedScene = SetScene<MainMenuScene>();
        break;
    case GameState::GAME:
        changedScene = SetScene<GameScene>();
        break;
    case GameState::NONE:
        d2e::Log::Error("Trying to change game state to no state.");
        DEBUG_BREAK();
        break;
    default:
        d2e::Log::Error("Trying to change game state to an unhandled state with ID: {}", static_cast<uint32_t>(mGameState));
        break;
    }

    if (changedScene)
    {
        mGameState = newState;
    }
}

} // Namespace d2eGame.