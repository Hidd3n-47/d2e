#pragma once

#include "d2e/ES/IComponent.h"

#include "d2e/Rendering/SpriteManager.h"

namespace d2e
{

class Sprite : public IComponent
{
public:
    Sprite() = default;
    Sprite(const spriteId spriteId);
    Sprite(const std::filesystem::path& filepath);

    void Update(const float dt) override;
    void Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera) override;

    [[nodiscard]] Vec2 GetHalfExtentsWorldSpace();

    D2E_COMPONENT("Sprite")
private:
    spriteId mSpriteId{ 0 };
};

} // Namespace d2e.