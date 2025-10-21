#pragma once

#include "Prefabs/Player.h"

namespace d2eNet
{
class Client;
} // Namespace d2eNet.

namespace d2eGame
{

enum class GameState
{
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
private:
    Player mPlayer;

    GameState mGameState = GameState::LOADING_LEVEL;

    void CreateWall(d2e::WeakRef<d2eNet::Client> client, const d2e::Vec2 translation, const d2e::Vec2 halfExtents);
};

} // Namespace d2e.
