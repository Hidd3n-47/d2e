#include "Player.h"

#include <d2e/Es/Components/Movement.h>
#include <d2e/ES/Components/CircleSprite.h>

#include "d2e/ES/Components/BoxCollider.h"

namespace d2eGame
{

void Player::CreatePrefab(d2e::WeakRef<d2e::Scene> scene)
{
    mGameObject = scene->CreateGameObject();

    (void)mGameObject->AddComponent<d2e::CircleSprite>();
    (void)mGameObject->AddComponent<d2e::BoxCollider>();
    auto movement = mGameObject->AddComponent<d2e::Movement>();

    movement->SetSpeed(0.2f);
}

} // Namespace d2eGame.