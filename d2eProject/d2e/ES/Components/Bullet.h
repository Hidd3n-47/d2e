#pragma once

#include "d2e/ES/IComponent.h"

#include "d2e/Rendering/SpriteManager.h"

namespace d2e
{
class Bullet : public IComponent
{
public:
    void Update(const float dt) override;

    D2E_COMPONENT("Bullet")
};

} // Namespace d2e.