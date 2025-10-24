#pragma once

#include "d2e/ES/IComponent.h"

namespace d2e
{

class GameObject;

class Movement : public IComponent
{
public:
    void Update(const float dt) override;

    bool  jumped;
    float xAxisDelta = 0.0f;

    float    speed        = 20.0f;
    uint16_t maxJumpCount = 2;
    uint16_t jumpCount    = 0;

    [[nodiscard]] std::string Serialize() const override;
    void Deserialize(const std::string& string) override;

    D2E_COMPONENT("Movement")
};

} // Namespace d2e.