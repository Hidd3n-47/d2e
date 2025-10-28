#pragma once

#include <cstdint>
#include <vector>

#include "d2e/Math/Vec2.h"
#include "d2e/Core/WeakRef.h"

namespace d2e
{
class Scene;
class GameObject;
}

namespace d2eGame
{

class BulletManager
{
public:
    void Init(d2e::WeakRef<d2e::Scene> scene, const d2e::WeakRef<d2e::GameObject> player);

    void ShootBullet(const d2e::Vec2 direction);

    static constexpr uint32_t NUM_BULLETS_IN_POOL{ 10 };
private:
    d2e::WeakRef<d2e::GameObject> mPlayer;

    std::vector<d2e::WeakRef<d2e::GameObject>> mPool;
    uint32_t mPoolIndex{ 0 };

    const float OFFSET{ 0.25f };
    const d2e::Vec2 GRAVITY{ 0.0f, -0.8f };
};

} // Namespace d2eGame.