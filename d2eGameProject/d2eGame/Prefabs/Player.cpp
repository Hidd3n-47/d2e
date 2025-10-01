#include "Player.h"

#include <d2e/Es/Components/Movement.h>
#include <d2e/ES/Components/CircleSprite.h>

#include "d2e/ES/Components/CircleCollider.h"

namespace d2eGame
{

void Player::CreatePrefab(d2e::WeakRef<d2e::Scene> scene)
{
    mGameObject = scene->CreateGameObject();

    mGameObject->AddComponent<d2e::CircleSprite>();
    mGameObject->AddComponent<d2e::CircleCollider>();
    mGameObject->AddComponent<d2e::Movement>();
}

} // Namespace d2eGame.