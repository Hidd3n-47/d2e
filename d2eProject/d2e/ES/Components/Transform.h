#pragma once

#include "d2e/ES/IComponent.h"
#include "d2e/Core/SerializationUtils.h"

namespace d2e
{

class Transform : public IComponent
{
public:
    Vec2  translation = Vec2{ 0.0f, 0.0f };
    Vec2  scale       = Vec2{ 1.0f, 1.0f};

    [[nodiscard]] inline std::string Serialize() const override
    {
        const std::string delimiter = "|";

        return SerializeUtils::Serialize(mSyncValuesOnUpdate) + SerializeUtils::Serialize(translation) + delimiter + SerializeUtils::Serialize(scale);
    }
    inline void Deserialize(const std::string& string) override
    {
        SerializeUtils::Deserialize(mSyncValuesOnUpdate, std::string{ string[0] });

        const size_t firstPosition  = string.find(SerializeUtils::DELIMITER);

        const std::string first  = string.substr(1, firstPosition);
        const std::string second = string.substr(firstPosition + 1);

        SerializeUtils::Deserialize(translation, first);
        SerializeUtils::Deserialize(scale, second);
    }

    D2E_COMPONENT("Transform")
};

} // Namespace d2e.