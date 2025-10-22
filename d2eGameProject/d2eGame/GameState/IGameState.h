#pragma once

#include <d2e/Core/WeakRef.h>

namespace d2e
{
class Scene;
} // Namespace d2e.

namespace d2eGame
{

class IGameState
{
public:
    virtual ~IGameState() = default;

    virtual void Init(d2e::WeakRef<d2e::Scene> scene) = 0;
    virtual void Update() = 0;
};

} // Namespace d2eGame.