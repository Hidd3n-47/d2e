#pragma once

#include "IGameState.h"

#include <d2e/Math/Vec2.h>

namespace d2eNet
{
class Client;
}

namespace d2eGame
{

class LoadingLevelState : public IGameState
{
public:
    void Init(d2e::WeakRef<d2e::Scene> scene) override;
    void Update() override;

    inline void LoadingCompleted() { mLoadingCompleted = true; }
private:
    d2e::WeakRef<d2e::Scene> mParent;

    bool mLoadingCompleted = false;

    void CreateWall(d2e::WeakRef<d2eNet::Client> client, const d2e::Vec2 translation, const d2e::Vec2 halfExtents);
};

} // Namespace d2eGame.