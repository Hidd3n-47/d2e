#pragma once

#include "d2e/ES/IComponent.h"

namespace d2e
{

class UiButton : public IComponent
{
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;

    void Update(const float dt) override;
    void Render(WeakRef<sf::RenderWindow> window) override;

    [[nodiscard]] inline Vec2 GetSize() const { return mSize; }
    inline void SetSize(const Vec2 size) { mSize = size; }
private:
    sf::RectangleShape mRectangle{ {10.0f, 10.0f} };

    Vec2 mSize{ 10.0f, 10.0f };
};

} // Namespace d2e.