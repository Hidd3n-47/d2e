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
    BATTLE_COUNTDOWN,
    PLAYING,
    PLAYER_KILLED
};

class GameScene : public d2e::Scene
{
public:
    void InitScene() override;
    void SceneUpdate() const override;

    void ChangeGameState(const GameState state);

    [[nodiscard]] inline Player& GetPlayer()      { return mPlayer; }
    [[nodiscard]] inline Player& GetOtherPlayer() { return mOtherPlayer; }

    [[nodiscard]] inline bool ArePlayersSyncedAcrossNetwork() const { return mPlayersSyncedAcrossNetwork; }
    inline void SetPlayersSyncedAcrossNetwork(const bool synced = true) { mPlayersSyncedAcrossNetwork = true; }

    [[nodiscard]] inline d2e::WeakRef<d2e::GameObject> GetScoreObject() const { return mScoreObject; }
private:
    Player mPlayer;
    Player mOtherPlayer;

    bool mPlayersSyncedAcrossNetwork = false;

    GameState   mState     = GameState::NONE;
    IGameState* mGameState = nullptr;

    d2e::WeakRef<d2e::GameObject> mScoreObject;
};

} // Namespace d2e.
