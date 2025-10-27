#include "WaitingForPlayersState.h"

#include <d2e/Core/Engine.h>

#include <d2eNet/Core/Client.h>

#include "d2e/Es/Components/Transform.h"
#include "d2e/Input/InputManager.h"
#include "Scene/GameScene.h"

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
    d2e::WeakRef<d2e::InputManager> inputManager = d2e::Engine::Instance()->GetInputManager();
    if (inputManager->IsMousePressed(sf::Mouse::Button::Left))
    {
        Player& player = mGameScene->GetPlayer();

        const d2e::Vec2 mousePos = inputManager->GetMousePositionWorldSpace();
        const d2e::Vec2 playerPos = player.GetGameObject()->GetComponent<d2e::Transform>()->translation;

        d2e::Vec2 direction = mousePos - playerPos;
        direction.Normalize();
        player.GetBulletManager().ShootBullet(direction);
    }
}

} // Namespace d2eGame.