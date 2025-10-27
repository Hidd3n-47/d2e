#pragma once

#include "d2e/ES/IComponent.h"

#include "d2e/Rendering/SpriteManager.h"

namespace d2e
{

class Text : public IComponent
{
public:
    void Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera) override;

    void SetText(const std::string& text);

    inline void SetTextSize(const uint32_t size) { mTextSize = size; }

    D2E_COMPONENT("Text")
private:
    std::string mTextString{ "0" };
    sf::Text    mText{ Engine::GAME_FONT, mTextString, 7 };

    uint32_t    mTextSize = 30.0f;
};

} // Namespace d2e.