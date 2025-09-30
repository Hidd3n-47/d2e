#pragma once

namespace d2e
{

struct Vec2
{
public:
    Vec2() = default;
    explicit Vec2(const float scalar) : x{ scalar }, y{ scalar } {}
    explicit Vec2(const float x, const float y) : x{ x }, y{ y } {}

    float x = 0.0f, y = 0.0f;

    Vec2 operator+(const Vec2 rhs) const
    {
        return Vec2{ x + rhs.x, y + rhs.y };
    }

    Vec2 operator-(const Vec2 rhs) const
    {
        return Vec2{ x - rhs.x, y - rhs.y };
    }

    Vec2 operator*(const Vec2 rhs) const
    {
        return Vec2{ x * rhs.x, y * rhs.y };
    }
};

}