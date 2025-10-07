#include "d2ePch.h"
#include "Random.h"

#include <ctime>

namespace d2e
{

std::mt19937 Random::mRandomEngine = std::mt19937(time(nullptr));

int Random::GetRandomIntBetween(const int lowerBoundInclusive, const int upperBoundExclusive)
{
    return lowerBoundInclusive + mod(GetRandomInt(), upperBoundExclusive - lowerBoundInclusive);
}

int Random::mod(const int value, const int mod)
{
    const int ans = value % mod;

    return ans < 0 ? ans + mod : ans;
}

} // Namespace d2e.