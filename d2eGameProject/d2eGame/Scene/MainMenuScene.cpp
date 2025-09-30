#include "MainMenuScene.h"

#include <d2e/ES/Scene.h>
#include <d2e/core/Engine.h>
#include <d2e/ES/Components/UiButton.h>

namespace d2eGame
{

MainMenuScene::MainMenuScene()
{
    mScene = d2e::Engine::Instance()->CreateScene();
}

void MainMenuScene::InitGameScene()
{
    d2e::WeakRef<d2e::GameObject>   playButtonObject = mScene->CreateGameObject();
    d2e::WeakRef<d2e::UiButton>     playButton = playButtonObject->AddComponent<d2e::UiButton>();

    const d2e::Vec2 windowSize = d2e::Engine::Instance()->GetWindowSize();

    playButton->SetSize(windowSize * d2e::Vec2{ 0.35f, 0.1f });
    playButtonObject->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.5f, 0.2f } - playButton->GetSize() * d2e::Vec2{ 0.5f, 0.5f };
}


} // Namespace d2eGame.