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
    PLAYER_KILLED,
    PLAYER_DISCONNECTED
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

    void IncreaseScore(const d2e::Ulid playerWhoDiedUlid);
    inline void GetPlayerScores(uint32_t& player1Score, uint32_t& player2Score) const { player1Score = mPlayer1Score; player2Score = mPlayer2Score; }
private:
    Player mPlayer;
    Player mOtherPlayer;

    uint32_t mPlayer1Score { 0 };
    uint32_t mPlayer2Score { 0 };

    bool mPlayersSyncedAcrossNetwork = false;

    GameState   mState     = GameState::NONE;
    IGameState* mGameState = nullptr;

    d2e::WeakRef<d2e::GameObject> mScoreObject;
};

} // Namespace d2e.
