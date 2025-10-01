#pragma once

#include "d2e/Es/IComponent.h"

namespace d2e
{

class GameObject;

class Movement : public IComponent
{
public:
    void Update(const float dt) override;

    [[nodiscard]] inline float GetSpeed() const { return mSpeed; }
    inline void SetSpeed(const float speed) { mSpeed = speed; }
private:
    float mSpeed = 1.0f;
};

} // Namespace d2e.