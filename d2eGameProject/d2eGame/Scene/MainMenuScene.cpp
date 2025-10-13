#include "MainMenuScene.h"

#include <d2e/ES/Scene.h>
#include <d2e/core/Engine.h>
#include <d2e/ES/Components/UiButton.h>
#include <d2e/ES/Components/Transform.h>

#include "Core/GameManager.h"

namespace d2eGame
{

MainMenuScene::MainMenuScene()
{
    mScene = d2e::Engine::Instance()->CreateScene();
}

void MainMenuScene::InitGameScene()
{
    const d2e::Vec2 windowSize = d2e::Engine::Instance()->GetWindowSize();

    const d2e::spriteId hostSpriteId = d2e::SpriteManager::Instance()->LoadTexture("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Host.png");
    const d2e::spriteId joinSpriteId = d2e::SpriteManager::Instance()->LoadTexture("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Join.png");
    const d2e::spriteId exitSpriteId = d2e::SpriteManager::Instance()->LoadTexture("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Exit.png");

    d2e::WeakRef<d2e::GameObject>   hostButtonObject = mScene->CreateGameObject();
    d2e::WeakRef<d2e::UiButton>     hostButton = hostButtonObject->AddComponent<d2e::UiButton>(hostSpriteId, []() { d2e::Engine::Instance()->GetLog()->Debug("testing"); });
    hostButtonObject->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.5f, 0.2f };

    d2e::WeakRef<d2e::GameObject>   joinButtonObject = mScene->CreateGameObject();
    d2e::WeakRef<d2e::UiButton>     joinButton = joinButtonObject->AddComponent<d2e::UiButton>(joinSpriteId, []() { GameManager::Instance()->JoinOnlineGame(); });
    joinButtonObject->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.5f, 0.4f };

    d2e::WeakRef<d2e::GameObject>   exitButtonObject = mScene->CreateGameObject();
    d2e::WeakRef<d2e::UiButton>     exitButton = exitButtonObject->AddComponent<d2e::UiButton>(exitSpriteId, []() { d2e::Engine::Instance()->CloseGame(); });
    exitButtonObject->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.5f, 0.6f };
}


} // Namespace d2eGame.