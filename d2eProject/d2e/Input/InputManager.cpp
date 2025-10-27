#include "d2ePch.h"
#include "InputManager.h"

#include "Core/Engine.h"

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

Vec2 InputManager::GetMousePositionWorldSpace() const
{
    const OrthoCamera& camera = Engine::Instance()->GetCamera();

    return camera.ScreenToWorld(mMousePosition);
}

} // Namespace d2e.
