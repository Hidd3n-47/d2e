#pragma once

#include "WeakRef.h"

namespace std
{

template <typename T>
struct hash<d2e::WeakRef<T>>
{
    [[nodiscard]] inline size_t operator()(const d2e::WeakRef<T>& ref) const noexcept
    {
        return std::hash<void*>{}(ref.GetRawPtr());
    }
};

} // Namespace std.