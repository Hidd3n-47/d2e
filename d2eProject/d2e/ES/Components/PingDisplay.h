#pragma once

#include "d2e/ES/IComponent.h"

#include "d2e/Rendering/SpriteManager.h"

namespace d2e
{

class PingDisplay : public IComponent
{
public:
    void Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera) override;

    std::string Serialize() const override;
    void Deserialize(const std::string& string) override;

    D2E_COMPONENT("PingDisplay")

    static constexpr uint32_t PING_CACHE_COUNT = 30;
private:
    uint64_t mPingCache[PING_CACHE_COUNT] { 0 };
    uint32_t mPingIndex = 0;

    std::string mTextString{ "0" };
    sf::Text    mText{ Engine::GAME_FONT, mTextString, 4 };
};

} // Namespace d2e.