#pragma once

#include "Prefabs/Player.h"

namespace d2eGame
{

class GameScene : public d2e::Scene
{
public:
    void InitScene() override;
    void SceneUpdate() const override;

private:
    Player mPlayer;
};

} // Namespace d2e.
