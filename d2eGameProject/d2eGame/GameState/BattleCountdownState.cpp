#include "BattleCountdownState.h"

#include <d2e/Core/Engine.h>
#include <d2e/ES/Components/BattleTimer.h>

#include <d2eNet/Core/Client.h>

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

    d2e::WeakRef<d2e::BattleTimer> timer = mGameScene->GetGameObject(d2e::Engine::BATTLE_TIMER_ULID)->GetComponent<d2e::BattleTimer>();
    timer->OnTimerCompletedCallback = [&] { BattleStarted(); };
    timer->StartTimer();
}

void BattleCountdownState::Update()
{
    if (mBattleStarted)
    {
        mGameScene->ChangeGameState(GameState::PLAYING);
    }
}


} // Namespace d2eGame.