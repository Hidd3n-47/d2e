#include "PlayerDisconnectedState.h"

#include <d2e/Core/Engine.h>
#include <d2e/ES/Components/Text.h>

#include "Core/GameManager.h"
#include "Scene/GameScene.h"
#include "Scene/MainMenuScene.h"

namespace d2eGame
{

void PlayerDisconnectedState::Init(d2e::WeakRef<d2e::Scene> scene)
{
    mGameScene = scene.Cast<GameScene>();

    mText = mGameScene->CreateGameObject();
    d2e::WeakRef<d2e::Text> text = mText->AddComponent<d2e::Text>();

    uint32_t player1Score, player2Score;
    mGameScene->GetPlayerScores(player1Score, player2Score);

    std::string string = std::format("Player disconnected!\nFinal score: {} - {}", player1Score, player2Score);

    text->SetText(string);
    text->SetTextSize(200);

    mTimer = TIME_TILL_MAIN_MENU;
}

void PlayerDisconnectedState::Update()
{
    mTimer -= d2e::Engine::Instance()->GetDeltaTime();

    if (mTimer <= 0.0f)
    {
        mGameScene->GetScoreObject()->GetComponent<d2e::Text>()->ShouldRender(false);

        GameManager::Instance()->ChangeState(ApplicationState::MAIN_MENU);
    }
}

} // Namespace d2eGame.