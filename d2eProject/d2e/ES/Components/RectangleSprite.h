#pragma once

#include "d2e/Es/IComponent.h"

namespace d2e
{

class GameObject;

class RectangleSprite : public IComponent
{
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;

    void Update(const float dt) override;
    void Render(WeakRef<sf::RenderWindow> window) override;

    sf::RectangleShape mRectangle { {10.0f, 10.0f} };
};

} // Namespace d2e.