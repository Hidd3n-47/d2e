#include "GameScene.h"

#include <d2e/ES/Scene.h>
#include <d2e/core/Engine.h>

#include "d2e/ES/Components/RectangleSprite.h"

namespace d2eGame
{

GameScene::GameScene()
{
    mScene = d2e::Engine::Instance()->CreateScene();
}

void GameScene::InitGameScene()
{
    const d2e::Vec2 windowSize = d2e::Engine::Instance()->GetWindowSize();

    {
        d2e::WeakRef<d2e::GameObject>       leftSideObject = mScene->CreateGameObject();
        d2e::WeakRef<d2e::RectangleSprite>  leftSideSprite = leftSideObject->AddComponent<d2e::RectangleSprite>();

        leftSideSprite->SetColor(sf::Color::Cyan);
        leftSideSprite->SetSize(windowSize * d2e::Vec2{ 0.5f, 1.0f });
    }

    {
        d2e::WeakRef<d2e::GameObject>       rightSideObject = mScene->CreateGameObject();
        d2e::WeakRef<d2e::RectangleSprite>  rightSideSprite = rightSideObject->AddComponent<d2e::RectangleSprite>();

        rightSideSprite->SetSize(windowSize * d2e::Vec2{ 0.5f, 1.0f });
        rightSideSprite->SetColor(sf::Color::Green);
        rightSideObject->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.5f, 0.0f };
    }

    {
        d2e::WeakRef<d2e::GameObject>       floorObject = mScene->CreateGameObject();
        d2e::WeakRef<d2e::RectangleSprite>  floorSprite = floorObject->AddComponent<d2e::RectangleSprite>();

        floorSprite->SetSize(windowSize * d2e::Vec2{ 1.0f, 0.05f });
        floorSprite->SetColor(sf::Color::Black);
        floorObject->GetComponent<d2e::Transform>()->translation = windowSize * d2e::Vec2{ 0.0f, 0.975f };
    }

    mPlayer.CreatePrefab(mScene);
}
} // Namespace d2eGame.