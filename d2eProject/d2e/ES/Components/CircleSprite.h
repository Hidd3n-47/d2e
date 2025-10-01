#pragma once

#include "d2e/Es/IComponent.h"

namespace d2e
{

class GameObject;

class CircleSprite : public IComponent
{
public:
    void Render(WeakRef<sf::RenderWindow> window) override;

    inline void SetRadius(const float radius) { mRadius = radius; }
    inline void SetColor(const sf::Color color) { mCircle.setFillColor(color); }
private:
    sf::CircleShape mCircle{ 10.0f };

    float mRadius = 10.0f;
};

} // Namespace d2e.