#pragma once

#include "d2e/Core/Engine.h"
#include "d2e/ES/IComponent.h"

namespace d2e
{

class BattleTimer : public IComponent
{
public:

    void Update(const float dt) override;
    void Render(WeakRef<sf::RenderWindow> window) override;

    static constexpr float TIME_TO_COUNTDOWN_SECONDS = 3.0f;

    std::function<void()> OnTimerCompletedCallback;
    D2E_COMPONENT("BattleTimer")
private:
    float mTimer = TIME_TO_COUNTDOWN_SECONDS;
    std::string mTextString;
    sf::Text mText{ Engine::GAME_FONT };
};

}
