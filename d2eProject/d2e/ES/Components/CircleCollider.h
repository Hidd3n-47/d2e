#pragma once

#include <d2e/ES/IComponent.h>

namespace d2e
{

class CircleCollider : public IComponent
{
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;
    void OnComponentRemoved() override;

    [[nodiscard]] inline float GetRadius() const { return mRadius; }

    void OnCollisionEnter();

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
};

} // Namespace d2e.