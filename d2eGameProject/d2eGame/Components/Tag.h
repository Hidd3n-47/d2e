#pragma once

#include <d2e/ES/IComponent.h>

namespace d2eGame
{

enum class ComponentTag : uint8_t
{
    NONE,
    WALL
};

struct Tag : public d2e::IComponent
{
    ComponentTag tag = ComponentTag::NONE;
};

} // Namespace d2e.