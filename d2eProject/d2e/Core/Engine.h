#pragma once

#include "Core/WeakRef.h"


namespace sf
{
class RenderWindow;
}

namespace d2e
{

class InputManager;

class Engine
{
public:
    [[nodiscard]] inline static WeakRef<Engine> Instance() { return WeakRef{ mInstance.get() }; }

    void Init();
    void Run() const;
    void Destroy();
private:
    static std::unique_ptr<Engine> mInstance;

    std::unique_ptr<sf::RenderWindow>   mWindow;
    std::unique_ptr<InputManager>       mInputManager;

    void Update() const;
    void Render() const;
};

} // Namespace d2e.
