#pragma once

#include "d2e/ES/IComponent.h"

#include "d2e/Rendering/SpriteManager.h"

namespace d2e
{


class PingDisplay : public IComponent
{
public:
    //PingDisplay()
    //    : mPing(sf::Font{ "E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Fonts/Roboto" }, mText, 10)
    //{}
    void Render(WeakRef<sf::RenderWindow> window) override;

    std::string Serialize() const override;
    void Deserialize(const std::string& string) override;

    D2E_COMPONENT("PingDisplay")
private:
    //sf::Text mPing;
    uint64_t mPing;

    std::string mText{ "foo" };
};

} // Namespace d2e.