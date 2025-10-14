#pragma once

namespace d2e::SerializeUtils
{

template <typename T>
[[nodiscard]] inline std::string Serialize(const T value)
{
    __debugbreak();
    return "";
}

template<>
inline std::string Serialize(const uint8_t value)
{
    return std::to_string(value);
}

template<>
inline std::string Serialize(const uint16_t value)
{
    return std::to_string(value);
}

template<>
inline std::string Serialize(const float value)
{
    return std::to_string(value);
}

template<>
inline std::string Serialize(const Vec2 value)
{
    return std::to_string(value.x) + "," + std::to_string(value.y);
}

template<>
inline std::string Serialize(const bool value)
{
    return std::to_string(value);
}

template <typename T>
inline void Deserialize(T& value, const std::string& str)
{
    __debugbreak();
}

template<>
inline void Deserialize(uint8_t& value, const std::string& str)
{
    value = static_cast<uint8_t>(std::stoul(str));
}

template<>
inline void Deserialize(uint16_t& value, const std::string& str)
{
    value = static_cast<uint16_t>(std::stoul(str));
}

template<>
inline void Deserialize(float& value, const std::string& str)
{
    value = std::stof(str);
}

template<>
inline void Deserialize(Vec2& value, const std::string& str)
{
    const size_t delimiterPosition = str.find(',');

    value.x = std::stof(str.substr(0, delimiterPosition));
    value.y = std::stof(str.substr(delimiterPosition + 1));
}

template<>
inline void Deserialize(bool& value, const std::string& str)
{
    value = std::stoi(str);
}

}