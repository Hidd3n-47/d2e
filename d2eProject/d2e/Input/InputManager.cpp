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

} // Namespace d2e.
