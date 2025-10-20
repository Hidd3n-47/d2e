#include "GameManager.h"

#include <iostream>

#include "src/Defines.h"
#include "Scene/GameScene.h"
#include "Scene/MainMenuScene.h"

namespace d2eGame
{

std::unique_ptr<GameManager> GameManager::mInstance = std::make_unique<GameManager>();

GameManager::GameManager()
{
    DEBUG(mLog = std::make_unique<d2e::Log>("d2e Game"));
}

GameManager::~GameManager()
{
    delete mCurrentScene;
}

void GameManager::Init()
{
    ChangeState(GameState::MAIN_MENU);

    d2e::Engine::Instance()->SetOnLevelLoadCompleteCallback([] { Instance()->ChangeState(GameState::GAME); });

    GAME_LOG("Game initialized.");
}

void GameManager::ChangeState(const GameState newState)
{
    if (mGameState == newState)
    {
        GAME_WARN("Trying to change game state to the same state.");
        return;
    }

    switch (newState)
    {
    case GameState::MAIN_MENU:
        SetScene<MainMenuScene>();
        break;
    case GameState::LOADING_GAME:
        GAME_LOG("Loading game.");
        SetScene<GameScene>();
        break;
    case GameState::GAME:
        GAME_LOG("Game successfully loaded.");
        mCurrentScene->SetSceneLoaded(true);
        break;
    case GameState::NONE:
        GAME_ERROR("Trying to change game state to no state.");
        DEBUG_BREAK();
        break;
    default:
        GAME_ERROR("Trying to change game state to an unhandled state with ID: {}", static_cast<uint32_t>(mGameState));
        break;
    }

    mGameState = newState;
}

void GameManager::JoinOnlineGame()
{
    std::cout << "Enter the IP of the server: ";

    int ip1, ip2, ip3, ip4;
    std::cin >> ip1 >> ip2 >> ip3 >> ip4;

    d2e::Engine::Instance()->ConnectClientToServer(ip1, ip2, ip3, ip4, 7777);

    ChangeState(GameState::LOADING_GAME);
}

} // Namespace d2eGame.