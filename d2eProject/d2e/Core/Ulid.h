#pragma once

#include <cstdint>

namespace d2e
{

class Ulid
{
public:
    Ulid();
    constexpr explicit Ulid(const uint64_t ulid) : mUlid{ ulid } { /* Empty. */ }

    inline operator uint64_t() const
    {
        return mUlid;
    }

    inline bool operator==(const Ulid& other) const
    {
        return mUlid == other.mUlid;
    }
private:
    uint64_t mUlid;

    [[nodiscard]] static uint32_t GetTimeFromEpoch();

    [[nodiscard]] static uint32_t GetRandomNumber();
};

} // Namespace d2eNet.

template <>
struct std::hash<d2e::Ulid>
{
    [[nodiscard]] inline size_t operator()(const d2e::Ulid& ulid) const noexcept
    {
        return std::hash<uint64_t>()(ulid);
    }
};