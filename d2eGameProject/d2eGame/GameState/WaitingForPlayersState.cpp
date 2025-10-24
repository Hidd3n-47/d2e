#include "WaitingForPlayersState.h"

#include <d2e/Core/Engine.h>

#include <d2eNet/Core/Client.h>

#include "Scene/GameScene.h"

#include "d2e/Input/InputManager.h"
#include "d2e/ES/Components/Movement.h"

namespace d2eGame
{

void WaitingForPlayersState::Init(d2e::WeakRef<d2e::Scene> scene)
{
    mGameScene = scene.Cast<GameScene>();

    const bool player1 = d2e::Engine::Instance()->GetClient()->GetId() == 1;

    mGameScene->GetPlayer().CreatePrefab(scene, player1);

    if (!player1)
    {
        mGameScene->GetOtherPlayer().CreatePrefab(scene, true);
    }
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
        if (inputManager->IsKeyPressed(sf::Keyboard::Key::Space))
        {
            jumped = true;
        }

        movement->xAxisDelta = xAxisDelta;
        movement->jumped = jumped;
}

} // Namespace d2eGame.