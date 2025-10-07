#pragma once

#include <random>

namespace d2e
{

class Random
{
public:
    static int GetRandomIntBetween(const int lowerBoundInclusive, const int upperBoundExclusive);

    inline static int GetRandomInt() { return static_cast<int>(mRandomEngine()); }
private:
    static std::mt19937 mRandomEngine;

    static int mod(const int value, const int mod);
};

} // Namespace d2e.
