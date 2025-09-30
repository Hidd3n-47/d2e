#pragma once

#include "d2e/Core/WeakRef.h"

namespace d2e
{
class Scene;
}

namespace d2eGame
{

class IGameScene
{
public:
    virtual ~IGameScene() = default;

    virtual void InitGameScene() { }

    d2e::WeakRef<d2e::Scene> GetScene() const { return mScene; }

protected:
    d2e::WeakRef<d2e::Scene> mScene;
};

} // Namespace d2eGame.
