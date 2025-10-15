#pragma once

#include "d2e/ES/IComponent.h"
#include "d2e/Core/SerializationUtils.h"

namespace d2e
{

class GameObject;

class CircleSprite : public IComponent
{
public:
    void Render(WeakRef<sf::RenderWindow> window) override;

    inline void SetRadius(const float radius) { mRadius = radius; }
    inline void SetColor(const sf::Color color) { mCircle.setFillColor(color); }

    [[nodiscard]] inline std::string Serialize() const override { return SerializeUtils::Serialize(mRadius); }
    inline void Deserialize(const std::string& string) override { SerializeUtils::Deserialize(mRadius, string.substr(1)); }

    D2E_COMPONENT("CircleSprite")
private:
    sf::CircleShape mCircle{ 10.0f };

    float mRadius = 10.0f;
};

} // Namespace d2e.