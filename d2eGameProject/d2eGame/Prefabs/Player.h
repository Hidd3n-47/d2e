#pragma once

#include <d2e/ES/IPrefab.h>
#include <d2e/Core/WeakRef.h>

namespace d2eGame
{

class Player : public d2e::IPrefab
{
public:
    void CreatePrefab(d2e::WeakRef<d2e::Scene> scene) override;

    [[nodiscard]] inline d2e::WeakRef<d2e::GameObject> GetGameObject() const { return mGameObject; }
private:
    d2e::WeakRef<d2e::GameObject> mGameObject;
};

} // Namespace d2eGame.