#pragma once

#include "IGameState.h"

namespace d2eGame
{
class GameScene;

class PlayerKilledState : public IGameState
{
public:
    void Init(d2e::WeakRef<d2e::Scene> scene) override;
    void Update() override;

    static constexpr float TIME_TILL_REMATCH = 4.0f;
private:
    d2e::WeakRef<GameScene> mGameScene;

    float mTimer { 0.0f };
};

} // Namespace d2eGame.