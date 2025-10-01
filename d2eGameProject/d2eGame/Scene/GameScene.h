#pragma once

#include "IGameScene.h"
#include "Prefabs/Player.h"

namespace d2eGame
{

class GameScene : public IGameScene
{
public:
    GameScene();

    void InitGameScene() override;

private:
    Player mPlayer;
};

} // Namespace d2e.
