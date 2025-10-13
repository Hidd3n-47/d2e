#pragma once

#include "d2e/Core/Rtti.h"
#include "d2e/ES/IComponent.h"

namespace d2e
{

class GameObject;

class Movement : public IComponent
{
public:
    void Update(const float dt) override;

    [[nodiscard]] inline float GetSpeed() const { return mSpeed; }
    inline void SetSpeed(const float speed) { mSpeed = speed; }

    D2E_COMPONENT(Movement)
private:
    float    mSpeed        = 1.0f;
    uint16_t mMaxJumpCount = 2;
    uint16_t mJumpCount    = 0;
};

} // Namespace d2e.