#pragma once

#include "d2e/Core/Rtti.h"
#include "d2e/ES/IComponent.h"

namespace d2e
{

class GameObject;

class RectangleSprite : public IComponent
{
public:
    void Render(WeakRef<sf::RenderWindow> window) override;

    [[nodiscard]] inline Vec2 GetHalfExtents() const { return mHalfExtents; }

    inline void SetHalfExtents(const Vec2 halfExtents) { mHalfExtents = halfExtents; }
    inline void SetColor(const sf::Color color) { mRectangle.setFillColor(color); }

    D2E_COMPONENT(RectangleSprite);
private:
    sf::RectangleShape mRectangle { { 10.0f, 10.0f } };

    Vec2 mHalfExtents{ 10.0f, 10.0f };
};

} // Namespace d2e.