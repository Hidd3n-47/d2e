#pragma once

#include <unordered_set>
#include <unordered_map>

namespace sf::Keyboard
{
enum class Key;
}

namespace d2e
{

class InputManager
{
public:
    InputManager()  = default;
    ~InputManager() = default;

    inline void StartFrame() { mKeyMapForSingleFrame.clear(); }

    void KeyDown(const sf::Keyboard::Key key);
    void KeyUp(const sf::Keyboard::Key key);

    [[nodiscard]] inline bool IsKeyDown(const sf::Keyboard::Key key)    const { if (!mKeyMap.contains(key)) return false; return mKeyMap.at(key); }
    [[nodiscard]] inline bool IsKeyPressed(const sf::Keyboard::Key key) const { return mKeyMapForSingleFrame.contains(key); }
private:
    std::unordered_map<sf::Keyboard::Key, bool> mKeyMap;
    std::unordered_set<sf::Keyboard::Key>       mKeyMapForSingleFrame;
};

} // Namespace d2e.