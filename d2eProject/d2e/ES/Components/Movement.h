#pragma once

#include "d2e/Es/IComponent.h"

namespace d2e
{

class GameObject;

class Movement : public IComponent
{
public:
    void OnComponentAdded(const WeakRef<GameObject> parent) override;
    void Update(const float dt) override;
private:
    float mSpeed = 1.0f;
};

} // Namespace d2e.