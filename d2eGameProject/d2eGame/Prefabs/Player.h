#pragma once

#include <d2e/ES/IPrefab.h>
#include <d2e/Core/WeakRef.h>
//#include <d2e/ES/Components/CircleSprite.h>

namespace d2eGame
{

class Player : public d2e::IPrefab
{
public:
    void CreatePrefab(d2e::WeakRef<d2e::Scene> scene) override;
private:
    d2e::WeakRef<d2e::GameObject> mGameObject;

    //d2e::WeakRef<d2e::CircleSprite> mVisual;
};

} // Namespace d2eGame.