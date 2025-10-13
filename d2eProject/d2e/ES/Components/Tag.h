#pragma once

#include "d2e/Core/Rtti.h"
#include "d2e/ES/IComponent.h"

namespace d2e
{

enum class ComponentTag : uint8_t
{
    NONE,
    WALL
};

struct Tag : public IComponent
{
    ComponentTag tag = ComponentTag::NONE;

    D2E_COMPONENT(Tag);
};

} // Namespace d2e.
