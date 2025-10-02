#pragma once

#include <d2e/ES/IComponent.h>

namespace d2e
{


// todo remove mFixed if its not going to be used for circle colliders.
class CircleCollider : public IComponent
{
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;
    void OnComponentRemoved() override;

    //[[nodiscard]] inline bool IsFixed() const { return mFixed; }
    [[nodiscard]] inline float GetRadius() const { return mRadius; }

    //inline void SetIsFixed(const bool fixed) { mFixed = fixed; }
    inline void SetRadius(const float radius) { mRadius = radius; }
#ifdef DEV_CONFIGURATION
public:
    void Render(WeakRef<sf::RenderWindow> window) override;
private:
    sf::CircleShape mDebugRender;
public:
#endif // DEV_CONFIGURATION.
private:
    float mRadius = 10.0f;
    //bool  mFixed    = false;
};

} // Namespace d2e.