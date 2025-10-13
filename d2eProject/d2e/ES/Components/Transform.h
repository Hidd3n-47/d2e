#pragma once

#include "d2e/Core/Rtti.h"
#include "d2e/ES/IComponent.h"

namespace d2e
{

class Transform : public IComponent
{
public:
    Vec2  translation = Vec2{ 0.0f, 0.0f };
    Vec2  scale       = Vec2{ 1.0f, 1.0f};

    D2E_COMPONENT(Transform)
};



} // Namespace d2e.