#pragma once

#include <numbers>

namespace d2e
{

class Math
{
public:
    inline static float DegToRads(const float degrees)
    {
        return degrees / 180.0f * std::numbers::pi_v<float>;
    }
};

} // Namespace d2e.