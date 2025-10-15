#pragma once

#include "d2e/ES/IComponent.h"
#include "d2e/Core/SerializationUtils.h"

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

    [[nodiscard]] inline std::string Serialize() const override { return SerializeUtils::Serialize(static_cast<uint8_t>(tag)); }
    inline void Deserialize(const std::string& string) override { uint8_t value;  SerializeUtils::Deserialize(value, string); tag = static_cast<ComponentTag>(value); }

    D2E_COMPONENT("Tag")
};

} // Namespace d2e.
