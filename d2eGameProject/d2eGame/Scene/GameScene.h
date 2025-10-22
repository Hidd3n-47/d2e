#pragma once

#include "Prefabs/Player.h"

namespace d2eNet
{
class Client;
} // Namespace d2eNet.

namespace d2eGame
{

class IGameState;

enum class GameState
{
    NONE,
    LOADING_LEVEL,
    WAITING_FOR_PLAYERS,
    LOADING_PLAYERS,
    PLAYING
};

class GameScene : public d2e::Scene
{
public:
    void InitScene() override;
    void SceneUpdate() const override;

    void ChangeGameState(const GameState state);

    [[nodiscard]] inline Player& GetPlayer() { return mPlayer; }
private:
    Player mPlayer;

    GameState   mState     = GameState::NONE;
    IGameState* mGameState = nullptr;
};

} // Namespace d2e.
