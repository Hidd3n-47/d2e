#include "GameManager.h"

#include <iostream>

#include <d2eNet/Core/Packet.h>

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

    GAME_LOG("Game initialized.");
}

void GameManager::ChangeState(const GameState newState)
{
    if (mGameState == newState)
    {
        GAME_WARN("Trying to change game state to the same state.");
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
        GAME_ERROR("Trying to change game state to no state.");
        DEBUG_BREAK();
        break;
    default:
        GAME_ERROR("Trying to change game state to an unhandled state with ID: {}", static_cast<uint32_t>(mGameState));
        break;
    }

    if (changedScene)
    {
        mGameState = newState;
    }
}

void GameManager::JoinOnlineGame()
{
    std::cout << "Enter the IP of the server: ";

    int ip1, ip2, ip3, ip4;
    std::cin >> ip1 >> ip2 >> ip3 >> ip4;

    d2e::Engine::Instance()->ConnectClientToServer(ip1, ip2, ip3, ip4, 7777);

    ChangeState(GameState::GAME);
}

} // Namespace d2eGame.