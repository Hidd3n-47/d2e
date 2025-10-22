#include "GameScene.h"

#include "src/Defines.h"

#include "GameState/LoadingLevelState.h"
#include "GameState/WaitingForPlayersState.h"

namespace d2eGame
{

void GameScene::InitScene()
{
    ChangeGameState(GameState::LOADING_LEVEL);
}

void GameScene::SceneUpdate() const
{
    mGameState->Update();
}

void GameScene::ChangeGameState(const GameState state)
{
    delete mGameState;

    switch (state)
    {
    case GameState::LOADING_LEVEL:
        mGameState = new LoadingLevelState();
        break;
    case GameState::WAITING_FOR_PLAYERS:
        mGameState = new WaitingForPlayersState();
        break;
    case GameState::LOADING_PLAYERS:
        break;
    case GameState::PLAYING:
        break;
    case GameState::NONE:
    default:
        GAME_WARN("Trying to change game state to an unhandled case.");
        break;
    }

    mState = state;
    mGameState->Init(d2e::WeakRef{ this }.Cast<d2e::Scene>());
}

} // Namespace d2eGame.