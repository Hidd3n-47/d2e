#include "GamePlayingState.h"

#include <d2e/Core/Engine.h>
#include <d2e/Input/InputManager.h>

#include <d2e/ES/Components/Movement.h>

#include <d2eNet/Core/Packet.h>
#include <d2eNet/Core/Client.h>

#include "Scene/GameScene.h"

namespace d2eGame
{

void GamePlayingState::Init(d2e::WeakRef<d2e::Scene> scene)
{
    mGameScene = scene.Cast<GameScene>();
}

void GamePlayingState::Update()
{
    d2e::WeakRef<d2e::GameObject> playerGameObject = mGameScene->GetPlayer().GetGameObject();
    d2e::WeakRef<d2e::Movement>   movement = playerGameObject->GetComponent<d2e::Movement>();

    const d2e::WeakRef<d2e::InputManager> inputManager = d2e::Engine::Instance()->GetInputManager();

    float xAxisDelta = 0.0f;
    if (inputManager->IsKeyDown(sf::Keyboard::Key::A))
    {
        xAxisDelta -= 1.0f;
    }
    if (inputManager->IsKeyDown(sf::Keyboard::Key::D))
    {
        xAxisDelta += 1.0f;
    }
    movement->xAxisDelta = xAxisDelta;

    if (inputManager->IsKeyPressed(sf::Keyboard::Key::Space))
    {
        ++movement->currentFrameJumpCount;
    }

    d2eNet::Packet packet{ false };
    packet.UpdateType<d2e::Movement>(playerGameObject->GetId(), movement->Serialize());
    d2e::Engine::Instance()->GetClient()->AddPacketToSend(packet);
}


} // Namespace d2eGame.