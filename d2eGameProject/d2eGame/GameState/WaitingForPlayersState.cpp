#include "WaitingForPlayersState.h"

#include <d2e/Core/Engine.h>
#include <d2e/Input/InputManager.h>
#include <d2e/ES/Components/Movement.h>

#include <d2eNet/Core/Client.h>

#include "Scene/GameScene.h"

namespace d2eGame
{

void WaitingForPlayersState::Init(d2e::WeakRef<d2e::Scene> scene)
{
    mGameScene = scene.Cast<GameScene>();

    d2e::WeakRef<d2eNet::Client> client = d2e::Engine::Instance()->GetClient();

    Player& player = mGameScene->GetPlayer();
    player.CreatePrefab(scene, client->GetId() == 1);
        // For two players.
//#define TWO_PLAYERS
//#ifdef TWO_PLAYERS
//    if (const bool isPlayer1 = client->GetId() == 1; isPlayer1)
//    {
//        player.CreatePrefab(scene, isPlayer1);
//
//        Player otherPlayer;
//        otherPlayer.CreatePrefab(scene, !isPlayer1);
//    }
//    else
//    {
//        Player otherPlayer;
//        otherPlayer.CreatePrefab(scene, !isPlayer1);
//
//        player.CreatePrefab(scene, isPlayer1);
//    }
//#else
//    mPlayer.CreatePrefab(sceneWeakRef, true);
//#endif
}

void WaitingForPlayersState::Update()
{
    d2e::WeakRef<d2e::GameObject> playerGameObject = mGameScene->GetPlayer().GetGameObject();
    d2e::WeakRef<d2e::Movement>   movement = playerGameObject->GetComponent<d2e::Movement>();

    float xAxisDelta = 0.0f;
    bool jumped = false;

    const d2e::WeakRef<d2e::InputManager> inputManager = d2e::Engine::Instance()->GetInputManager();
    if (inputManager->IsKeyDown(sf::Keyboard::Key::A))
    {
        xAxisDelta -= 1.0f;
    }
    if (inputManager->IsKeyDown(sf::Keyboard::Key::D))
    {
        xAxisDelta += 1.0f;
    }
    if (inputManager->IsKeyPressed(sf::Keyboard::Key::Space) )
    {
        jumped = true;
    }

    movement->xAxisDelta = xAxisDelta;
    movement->jumped     = jumped;

    d2eNet::Packet packet{ false };
    packet.UpdateType<d2e::Movement>(playerGameObject->GetId(), movement->Serialize());
    d2e::Engine::Instance()->GetClient()->AddPacketToSend(packet);
}

} // Namespace d2eGame.