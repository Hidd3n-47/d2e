#include "BattleCountdownState.h"

#include <d2e/Core/Engine.h>

#include <d2eNet/Core/Client.h>

#include "src/Defines.h"
#include "Scene/GameScene.h"

namespace d2eGame
{

void BattleCountdownState::Init(d2e::WeakRef<d2e::Scene> scene)
{
    mGameScene = scene.Cast<GameScene>();

    if (!mGameScene->ArePlayersSyncedAcrossNetwork())
    {
        mGameScene->GetPlayer().SyncPlayer();
        mGameScene->SetPlayersSyncedAcrossNetwork();
    }

    mTimer = TIME_TO_COUNTDOWN_SECONDS;
}

void BattleCountdownState::Update()
{
    mTimer -= d2e::Engine::Instance()->GetDeltaTime();
    GAME_LOG("Time till battle: {}", mTimer);

    if (mTimer < 0.0f)
    {
        mGameScene->ChangeGameState(GameState::PLAYING);
    }
}


} // Namespace d2eGame.