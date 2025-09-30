#pragma once

#include "IGameScene.h"

namespace d2eGame
{

class MainMenuScene : public IGameScene
{
public:
    MainMenuScene();

    void InitGameScene() override;
};

} // Namespace d2e.
