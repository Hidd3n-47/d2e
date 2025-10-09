#include "d2ePch.h"
#include "InputManager.h"


namespace d2e
{

void InputManager::KeyDown(const sf::Keyboard::Key key)
{
    mKeyMap[key] = true;

    mKeyMapForSingleFrame.emplace(key);
}

void InputManager::KeyUp(const sf::Keyboard::Key key)
{
    mKeyMap[key] = false;
}

void InputManager::MouseDown(const sf::Mouse::Button button)
{
    mMouseButtonMap[button] = true;

    mMouseButtonMapForSingleFrame.emplace(button);
}

void InputManager::MouseUp(const sf::Mouse::Button button)
{
    mMouseButtonMap[button] = false;
}

} // Namespace d2e.
