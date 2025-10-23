#pragma once

#include "d2e/ES/IComponent.h"

#include "d2e/Core/Engine.h"
#include "d2e/Core/SerializationUtils.h"

namespace d2e
{

class BattleTimer : public IComponent
{
public:
    inline void StartTimer() { mTimer = TIME_TO_COUNTDOWN_SECONDS; mTimerRunning = true; }
    void Update(const float dt) override;
    void Render(WeakRef<sf::RenderWindow> window) override;

    static constexpr float TIME_TO_COUNTDOWN_SECONDS = 3.0f;

    std::function<void()> OnTimerCompletedCallback;

    inline std::string Serialize() const override { return SerializeUtils::Serialize(mTimerRunning) + SerializeUtils::Serialize(mTimer); }
    inline void Deserialize(const std::string& string) override { SerializeUtils::Deserialize(mTimerRunning, std::string{ string[0] }); SerializeUtils::Deserialize(mTimer, string.substr(1)); }

    D2E_COMPONENT("BattleTimer")
private:
    float mTimer = 0.0f;
    bool  mTimerRunning = false;

    std::string mTextString;
    sf::Text    mText{ Engine::GAME_FONT };
};

}
