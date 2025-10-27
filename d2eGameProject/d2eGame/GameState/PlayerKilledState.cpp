#include "PlayerKilledState.h"

#include <d2e/Core/Engine.h>
#include <d2e/ES/Components/Text.h>

#include "Scene/GameScene.h"

namespace d2eGame
{

void PlayerKilledState::Init(d2e::WeakRef<d2e::Scene> scene)
{
    mGameScene = scene.Cast<GameScene>();

    mTimer = TIME_TILL_REMATCH;
}

void PlayerKilledState::Update()
{
    mTimer -= d2e::Engine::Instance()->GetDeltaTime();

    if (mTimer <= 0.0f)
    {
        mGameScene->GetScoreObject()->GetComponent<d2e::Text>()->ShouldRender(false);

        mGameScene->ChangeGameState(GameState::BATTLE_COUNTDOWN);
    }
}

} // Namespace d2eGame.