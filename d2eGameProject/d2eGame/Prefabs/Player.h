#pragma once

#include <d2e/ES/Scene.h>
#include <d2e/Core/WeakRef.h>

#include "Systems/BulletManager.h"

namespace d2eGame
{

class Player
{
public:
    void CreatePrefab(d2e::WeakRef<d2e::Scene> scene, const bool isPlayer1 = false);
    void SyncPlayer();

    void ResetPosition(const bool uploadToServer = false);

    [[nodiscard]] inline d2e::WeakRef<d2e::GameObject> GetGameObject() const { return mGameObject; }
    [[nodiscard]] inline BulletManager& GetBulletManager() { return mBulletManager; }

    static constexpr sf::Color PLAYER_1_COLOR{ 255,   0, 132, 255 };
    static constexpr sf::Color PLAYER_2_COLOR{  31, 160, 200, 255 };
private:
    d2e::WeakRef<d2e::GameObject> mGameObject;
    bool mPlayer1 = false;

    BulletManager mBulletManager;
};

} // Namespace d2eGame.