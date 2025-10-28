#pragma once

#include "IGameState.h"

namespace d2e
{
class GameObject;
} // Namespace d2e.

namespace d2eGame
{
class GameScene;

class PlayerDisconnectedState : public IGameState
{
public:
    void Init(d2e::WeakRef<d2e::Scene> scene) override;
    void Update() override;

    static constexpr float TIME_TILL_MAIN_MENU = 4.0f;
private:
    d2e::WeakRef<GameScene> mGameScene;
    d2e::WeakRef<d2e::GameObject> mText;

    float mTimer{ 0.0f };
};

} // Namespace d2eGame.