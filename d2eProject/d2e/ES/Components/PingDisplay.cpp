#include "d2ePch.h"
#include "PingDisplay.h"

#include "ES/GameObject.h"
#include "ES/Components/Transform.h"

namespace d2e
{

void PingDisplay::Render(WeakRef<sf::RenderWindow> window)
{
    static sf::Font f{ "E:/Programming/d2e/d2eGameProject/d2eGame/Assets/Fonts/Roboto/Roboto-Bold.ttf" };
    mText = std::to_string(mPing);

    sf::Text p(f, mText, 10);

    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    p.setPosition({ transform->translation.x, transform->translation.y});
    p.setCharacterSize(20);
    p.setString(mText);

    window->draw(p);
}

std::string PingDisplay::Serialize() const
{
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    return SerializeUtils::Serialize(static_cast<uint64_t>(now.time_since_epoch().count()));
}

void PingDisplay::Deserialize(const std::string& string)
{
    const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());

    uint64_t sentTime;
    SerializeUtils::Deserialize(sentTime, string);

    const std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> sentTimePoint{ std::chrono::milliseconds{sentTime} };

    mPing = std::chrono::milliseconds(now - sentTimePoint).count();
}
} // Namespace d2e.