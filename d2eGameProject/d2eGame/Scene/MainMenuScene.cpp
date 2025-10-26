#include "MainMenuScene.h"

#include <d2e/ES/Scene.h>
#include <d2e/core/Engine.h>

#include <d2e/ES/Components/UiButton.h>
#include <d2e/ES/Components/Transform.h>

#include "Core/GameManager.h"
#include "d2e/ES/Components/Sprite.h"

namespace d2eGame
{

void MainMenuScene::InitScene()
{
    const d2e::spriteId joinSpriteId = d2e::SpriteManager::Instance()->LoadTexture("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Join.png");
    const d2e::spriteId exitSpriteId = d2e::SpriteManager::Instance()->LoadTexture("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Exit.png");

    d2e::WeakRef<d2e::GameObject> joinButtonObject = CreateGameObject();
    d2e::WeakRef<d2e::UiButton>   joinButton = joinButtonObject->AddComponent<d2e::UiButton>(joinSpriteId, []() { GameManager::Instance()->JoinOnlineGame(); });
    joinButtonObject->GetComponent<d2e::Transform>()->translation = d2e::Vec2{ 0.0f, 2.0f };

    d2e::WeakRef<d2e::GameObject> exitButtonObject = CreateGameObject();
    d2e::WeakRef<d2e::UiButton>   exitButton = exitButtonObject->AddComponent<d2e::UiButton>(exitSpriteId, []() { d2e::Engine::Instance()->CloseGame(); });
    exitButtonObject->GetComponent<d2e::Transform>()->translation = d2e::Vec2{ 0.0f, -2.0f };

    // Testing asset.
    {
        d2e::WeakRef<d2e::GameObject> floorObject = CreateGameObject();
        const d2e::Ulid id = floorObject->GetId();

        d2e::WeakRef<d2e::Sprite> floorSprite = floorObject->AddComponent<d2e::Sprite>("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Floor.png");

        d2e::WeakRef<d2e::Transform> transform = floorObject->GetComponent<d2e::Transform>();
        transform->scale = d2e::Vec2{ 0.5f, 0.5f };
    }
}


} // Namespace d2eGame.