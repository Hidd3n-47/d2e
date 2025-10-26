#include "d2ePch.h"
#include "BattleTimer.h"

#include "Transform.h"
#include "ES/GameObject.h"

namespace d2e
{

void BattleTimer::Update(const float dt)
{
    if (!mTimerRunning)
    {
        return;
    }

    mTimer -= dt;

    if (mTimer <= 0.0f)
    {
        mTimerRunning = false;

        if (OnTimerCompletedCallback)
        {
            OnTimerCompletedCallback();
        }
    }
}

void BattleTimer::Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera)
{
    if (!mTimerRunning)
    {
        return;
    }

    mTextString = std::to_string(static_cast<int>(mTimer));
    mText.setString(mTextString);

    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    const Vec2 position = camera.PositionToScreenSpace(transform->translation);
    sf::FloatRect bounds = mText.getLocalBounds();

    mText.setPosition({ position.x - bounds.size.x * 0.5f, position.y - bounds.size.y });
    mText.setCharacterSize(500);

    window->draw(mText);
}

} // Namespace d2e.