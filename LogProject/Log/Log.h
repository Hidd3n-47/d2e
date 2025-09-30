#pragma once

#include <print>
#include <string>
#include <format>

namespace d2e
{

// todo Christian make it that you can add a name for the logger as well as the timestamp/other settings.
// todo Christian refine to be better.
class Log
{
public:
    template <typename... Args>
    static inline void Debug(const std::string_view msg, Args&&... args)
    {
        Print(msg, BLUE, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static inline void Warn(const std::string_view msg, Args&&... args)
    {
        Print(msg, ORANGE, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static inline void Error(const std::string_view msg, Args&&... args)
    {
        Print(msg, RED, std::forward<Args>(args)...);
    }

private:
    static constexpr const char* GREEN      = "\033[32m";
    static constexpr const char* BLUE       = "\033[94m";
    static constexpr const char* ORANGE     = "\033[38;5;208m";
    static constexpr const char* RED        = "\033[31m";
    static constexpr const char* DEFAULT    = "\033[0m";

    template <typename T>
    static inline auto ToFormatArg(T&& t)
    {
        if constexpr (std::is_convertible_v<T, std::string_view>)
        {
            return std::string(t);
        }
        else
        {
            return std::forward<T>(t);
        }
    }

    template <typename... Args>
    static inline void Print(const std::string_view msg, const char* color, Args&&... args)
    {
        if constexpr (sizeof...(args) == 0)
        {
            std::println("{}{}{}", color, msg, DEFAULT);
        }
        else
        {
            auto argsTuple = std::make_tuple(ToFormatArg(std::forward<Args>(args))...);

            auto formattedMsg = std::vformat(msg, std::apply([](auto&... vals) { return std::make_format_args(vals...); }, argsTuple));

            std::println("{}{}{}", color, formattedMsg, DEFAULT);
        }
    }


};

} // Namespace d2e.