#pragma once

#include "IGameState.h"

namespace d2eGame
{
class GameScene;

class BattleCountdownState : public IGameState
{
public:
    void Init(d2e::WeakRef<d2e::Scene> scene) override;
    void Update() override;

    inline void BattleStarted() { mBattleStarted = true; }
private:
    d2e::WeakRef<GameScene> mGameScene;
    bool mBattleStarted = false;
};

} // Namespace d2eGame.