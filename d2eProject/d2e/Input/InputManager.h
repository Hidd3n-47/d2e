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

    inline void StartFrame() { mKeyMapForSingleFrame.clear(); mMouseButtonMapForSingleFrame.clear(); }

    void KeyDown(const sf::Keyboard::Key key);
    void KeyUp(const sf::Keyboard::Key key);
    void MouseDown(const sf::Mouse::Button button);
    void MouseUp(const sf::Mouse::Button button);

    [[nodiscard]] inline bool IsKeyDown(const sf::Keyboard::Key key)         const { if (!mKeyMap.contains(key)) return false; return mKeyMap.at(key); }
    [[nodiscard]] inline bool IsMouseDown(const sf::Mouse::Button button)    const { if (!mMouseButtonMap.contains(button)) return false; return mMouseButtonMap.at(button); }
    [[nodiscard]] inline bool IsKeyPressed(const sf::Keyboard::Key key)      const { return mKeyMapForSingleFrame.contains(key); }
    [[nodiscard]] inline bool IsMousePressed(const sf::Mouse::Button button) const { return mMouseButtonMapForSingleFrame.contains(button); }
    [[nodiscard]] inline Vec2 GetMousePosition() const { return mMousePosition; }

    inline void SetMousePosition(const Vec2 mousePosition) { mMousePosition = mousePosition; }
private:
    std::unordered_map<sf::Keyboard::Key, bool> mKeyMap;
    std::unordered_set<sf::Keyboard::Key>       mKeyMapForSingleFrame;

    Vec2 mMousePosition;
    std::unordered_map<sf::Mouse::Button, bool> mMouseButtonMap;
    std::unordered_set<sf::Mouse::Button>       mMouseButtonMapForSingleFrame;
};

} // Namespace d2e.