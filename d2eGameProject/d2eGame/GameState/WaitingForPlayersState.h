#pragma once

#include "IGameState.h"

namespace d2eGame
{

class GameScene;

class WaitingForPlayersState : public IGameState
{
public:
    void Init(d2e::WeakRef<d2e::Scene> scene) override;
    void Update() override;
private:
    d2e::WeakRef<GameScene> mGameScene;
};

} // Namespace d2eGame.