#include "MainMenuScene.h"

#include <d2e/ES/Scene.h>
#include <d2e/core/Engine.h>
#include <d2e/ES/Components/RectangleSprite.h>

namespace d2eGame
{

MainMenuScene::MainMenuScene()
{
    mScene = d2e::Engine::Instance()->CreateScene();

    auto square = mScene->CreateGameObject();
    auto s = square->AddComponent<d2e::RectangleSprite>();
}

} // Namespace d2eGame.