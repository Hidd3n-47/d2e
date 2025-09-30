#include "GameScene.h"

#include <d2e/core/Engine.h>

namespace d2eGame
{

GameScene::GameScene()
{
    mScene = d2e::Engine::Instance()->CreateScene();
}

} // Namespace d2eGame.