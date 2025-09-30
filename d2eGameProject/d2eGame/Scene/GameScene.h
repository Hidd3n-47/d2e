#pragma once

#include "IGameScene.h"

namespace d2eGame
{

class GameScene : public IGameScene
{
public:
    GameScene();

    void InitGameScene() override;
};

} // Namespace d2e.
