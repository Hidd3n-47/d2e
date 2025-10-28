#include "GameScene.h"

#include <d2e/ES/Components/Text.h>

#include "src/Defines.h"

#include "GameState/GamePlayingState.h"
#include "GameState/PlayerKilledState.h"
#include "GameState/LoadingLevelState.h"
#include "GameState/BattleCountdownState.h"
#include "GameState/WaitingForPlayersState.h"
#include "GameState/PlayerDisconnectedState.h"

namespace d2eGame
{

void GameScene::InitScene()
{
    ChangeGameState(GameState::LOADING_LEVEL);

    mScoreObject = CreateGameObject();
    d2e::WeakRef<d2e::Text> text = mScoreObject->AddComponent<d2e::Text>();
    text->SetTextSize(500);
    text->ShouldRender(false);
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
        GAME_LOG("Changing game state: Loading game.");
        mGameState = new LoadingLevelState();
        break;
    case GameState::WAITING_FOR_PLAYERS:
        GAME_LOG("Changing game state: Waiting for players.");
        mGameState = new WaitingForPlayersState();
        break;
    case GameState::BATTLE_COUNTDOWN:
        GAME_LOG("Changing game state: Battle countdown.");
        mGameState = new BattleCountdownState();
        break;
    case GameState::PLAYING:
        GAME_LOG("Changing game state: Playing.");
        mGameState = new GamePlayingState();
        break;
    case GameState::PLAYER_KILLED:
        GAME_LOG("Changing game state: Killed Player.");
        mGameState = new PlayerKilledState();
        break;
    case GameState::PLAYER_DISCONNECTED:
        GAME_LOG("Changing game state: Player disconnected.");
        mGameState = new PlayerDisconnectedState();
        break;
    case GameState::NONE:
    default:
        GAME_WARN("Trying to change game state to an unhandled case.");
        break;
    }

    mState = state;
    mGameState->Init(d2e::WeakRef{ this }.Cast<d2e::Scene>());
}

void GameScene::IncreaseScore(const d2e::Ulid playerWhoDiedUlid)
{
    if (playerWhoDiedUlid == d2e::Engine::PLAYER_TWO_ULID)
    {
        ++mPlayer1Score;
    }
    else
    {
        ++mPlayer2Score;
    }

    std::string scoreString = std::format("{} - {}", mPlayer1Score, mPlayer2Score);

    d2e::WeakRef<d2e::Text> text = mScoreObject->GetComponent<d2e::Text>();
    text->SetText(scoreString);
    text->ShouldRender(true);
}

} // Namespace d2eGame.