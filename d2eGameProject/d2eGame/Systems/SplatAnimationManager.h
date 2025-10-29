#pragma once

#include <vector>

#include <d2e/Math/Vec2.h>
#include <d2e/Core/WeakRef.h>

#include <d2e/Rendering/SpriteManager.h>

namespace d2e
{
class Scene;
class GameObject;
} // Namespace d2e.

namespace d2eGame
{

class SplatAnimationManager
{
public:
    SplatAnimationManager()  = default;
    ~SplatAnimationManager() = default;

    [[nodiscard]] static inline d2e::WeakRef<SplatAnimationManager> Instance() { return d2e::WeakRef{ mInstance.get() }; }

    void InitPool(d2e::WeakRef<d2e::Scene> scene);

    void AddSplatAnimation(const d2e::Vec2 collisionPosition, const bool playerOne);

    constexpr static uint32_t POOL_SIZE{ 15 };
private:
    static std::unique_ptr<SplatAnimationManager> mInstance;

    std::vector<d2e::WeakRef<d2e::GameObject>> mPool;
    uint32_t mCurrentIndex{ 0 };

    d2e::spriteId mSpriteId{ 0 };

    static void SetTransformForAnimation(d2e::WeakRef<d2e::GameObject> gameObject, const d2e::Vec2 position);
};

} // Namespace d2eGame.