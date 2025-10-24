#pragma once

#include "d2e/ES/IComponent.h"

#include "d2e/Rendering/SpriteManager.h"

namespace d2e
{

class UiButton : public IComponent
{
public:
    UiButton() = default;
    UiButton(const spriteId spriteId);
    UiButton(const spriteId spriteId, const std::function<void()>& callback);

    void Update(const float dt) override;
    void Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera) override;

    inline void SetOnButtonPressedCallback(const std::function<void()>& callback) { mOnButtonPressed = callback; }

    D2E_COMPONENT("UiButton")
private:
    spriteId mSpriteId{ 0 };
    Vec2 mSize{ 10.0f, 10.0f };

    std::function<void()> mOnButtonPressed;
};

} // Namespace d2e.