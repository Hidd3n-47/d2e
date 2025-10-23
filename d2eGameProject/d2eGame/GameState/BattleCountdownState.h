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

    static constexpr float TIME_TO_COUNTDOWN_SECONDS = 3.0f;
private:
    d2e::WeakRef<GameScene> mGameScene;

    float mTimer = 0;
};

} // Namespace d2eGame.