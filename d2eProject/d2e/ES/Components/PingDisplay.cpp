#include "d2ePch.h"
#include "PingDisplay.h"

#include "ES/GameObject.h"
#include "ES/Components/Transform.h"

namespace d2e
{

void PingDisplay::Render(WeakRef<sf::RenderWindow> window)
{
    if (mPingIndex == 0)
    {
        uint64_t count = 0;
        for (const uint64_t ping : mPingCache)
        {
            count += ping;
        }

        mTextString = std::to_string(count / PING_CACHE_COUNT);
        mText.setString(mTextString);
    }

    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    mText.setPosition({ transform->translation.x, transform->translation.y});
    mText.setCharacterSize(20);

    window->draw(mText);
}

std::string PingDisplay::Serialize() const
{
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    return SerializeUtils::Serialize(mSyncValuesOnUpdate) + SerializeUtils::Serialize(static_cast<uint64_t>(now.time_since_epoch().count()));
}

void PingDisplay::Deserialize(const std::string& string)
{
    SerializeUtils::Deserialize(mSyncValuesOnUpdate, std::string{ string[0] });
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

    uint64_t sentTime;
    SerializeUtils::Deserialize(sentTime, string.substr(1));

    const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> sentTimePoint{ std::chrono::milliseconds{sentTime} };

    mPingCache[mPingIndex] = std::chrono::milliseconds(now - sentTimePoint).count();
    mPingIndex = (mPingIndex + 1) % PING_CACHE_COUNT;
}
} // Namespace d2e.