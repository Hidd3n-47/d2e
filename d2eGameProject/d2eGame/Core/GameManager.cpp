#include "GameManager.h"

#include <iostream>

#include <d2eNet/Core/Client.h>

#include "src/Defines.h"
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
    ChangeState(ApplicationState::MAIN_MENU);

    d2e::Engine::Instance()->SetOnPlayerTwoJoinedCallback([] { Instance()->OnPlayerTwoJoined(); });
    d2e::Engine::Instance()->SetOnPlayerTwoReadyCallback([] { Instance()->OnPlayerTwoReady(); });

    GAME_LOG("Game initialized.");
}

void GameManager::ChangeState(const ApplicationState newState)
{
    if (mApplicationState == newState)
    {
        GAME_WARN("Trying to change game state to the same state.");
        return;
    }

    switch (newState)
    {
    case ApplicationState::MAIN_MENU:
        SetScene<MainMenuScene>();
        break;
    case ApplicationState::GAME:
        GAME_LOG("Game State.");
        SetScene<GameScene>();
        break;
    case ApplicationState::NONE:
        GAME_ERROR("Trying to change game state to no state.");
        DEBUG_BREAK();
        break;
    default:
        GAME_ERROR("Trying to change game state to an unhandled state with ID: {}", static_cast<uint32_t>(mApplicationState));
        break;
    }

    mApplicationState = newState;
}

void GameManager::JoinOnlineGame()
{
    std::cout << "Enter the IP of the server: ";

    int ip1, ip2, ip3, ip4;
    std::cin >> ip1 >> ip2 >> ip3 >> ip4;

    d2e::Engine::Instance()->ConnectClientToServer(ip1, ip2, ip3, ip4, 7777);

    ChangeState(ApplicationState::GAME);
}

void GameManager::OnPlayerTwoReady() const
{
    GameScene* gameScene = dynamic_cast<GameScene*>(mCurrentScene);
    gameScene->ChangeGameState(GameState::BATTLE_COUNTDOWN);
}

void GameManager::OnPlayerTwoJoined() const
{
    GameScene* gameScene = dynamic_cast<GameScene*>(mCurrentScene);

    if (d2e::Engine::Instance()->GetClient()->GetId() == 1)
    {
        gameScene->GetOtherPlayer().CreatePrefab(d2e::WeakRef{ mCurrentScene }.Cast<d2e::Scene>(), false);
    }
    else
    {
        gameScene->GetPlayer().CreatePrefab(d2e::WeakRef{ mCurrentScene }.Cast<d2e::Scene>(), false);
        gameScene->GetOtherPlayer().CreatePrefab(d2e::WeakRef{ mCurrentScene }.Cast<d2e::Scene>(), true);

        {
            d2eNet::Packet p;
            p.AddStringToPacket(d2eNet::PacketLineType::PLAYER_TWO_READY);
            d2e::Engine::Instance()->GetClient()->AddPacketToSend(p);
        }
    }
}

} // Namespace d2eGame.