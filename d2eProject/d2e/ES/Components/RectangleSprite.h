#pragma once

#include "d2e/Es/IComponent.h"

namespace d2e
{

class GameObject;

class RectangleSprite : public IComponent
{
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;

    void Render(WeakRef<sf::RenderWindow> window) override;

    inline void SetSize(const Vec2 size) { mSize = size; }
    inline void SetColor(const sf::Color color) { mRectangle.setFillColor(color); }

private:
    sf::RectangleShape mRectangle { {10.0f, 10.0f} };

    Vec2 mSize{ 10.0f, 10.0f };
};

} // Namespace d2e.