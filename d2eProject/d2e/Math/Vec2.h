#pragma once

namespace d2e
{

struct Vec2
{
public:
    Vec2() = default;
    explicit Vec2(const float scalar) : x{ scalar }, y{ scalar } {}
    explicit Vec2(const float x, const float y) : x{ x }, y{ y } {}

    void Normalize()
    {
        if (const float len = sqrt(x * x + y * y); len != 0.0f)
        {
            x /= len;
            y /= len;
        }
    }

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

    Vec2 operator*(const float scalar) const
    {
        return Vec2{ x * scalar, y * scalar };
    }

    Vec2 operator+=(const Vec2 rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }
};

}