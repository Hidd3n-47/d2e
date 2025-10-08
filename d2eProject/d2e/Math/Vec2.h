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

    [[nodiscard]] inline float Magnitude() const
    {
        return sqrt(x * x + y * y);
    }

    inline void Normalize()
    {
        if (const float len = sqrt(x * x + y * y); len != 0.0f)
        {
            x /= len;
            y /= len;
        }
    }

    [[nodiscard]] inline static float Dot(const Vec2 lhs, const Vec2 rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    [[nodiscard]] inline Vec2 operator+(const Vec2 rhs) const
    {
        return Vec2{ x + rhs.x, y + rhs.y };
    }

    [[nodiscard]] inline Vec2 operator-(const Vec2 rhs) const
    {
        return Vec2{ x - rhs.x, y - rhs.y };
    }

    [[nodiscard]] inline Vec2 operator-(const float scalar) const
    {
        return Vec2{ x - scalar, y - scalar };
    }

    [[nodiscard]] inline Vec2 operator*(const Vec2 rhs) const
    {
        return Vec2{ x * rhs.x, y * rhs.y };
    }

    [[nodiscard]] inline Vec2 operator*(const float scalar) const
    {
        return Vec2{ x * scalar, y * scalar };
    }

    [[nodiscard]] inline Vec2 operator/(const float scalar) const
    {
        return Vec2{ x / scalar, y / scalar };
    }

    inline Vec2 operator+=(const Vec2 rhs)
    {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    inline Vec2 operator-=(const Vec2 rhs)
    {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    inline Vec2 operator-=(const float scalar)
    {
        x -= scalar;
        y -= scalar;

        return *this;
    }

    inline Vec2 operator*=(const float scalar)
    {
        x *= scalar;
        y *= scalar;

        return *this;
    }

    inline Vec2 operator/=(const float scalar)
    {
        x /= scalar;
        y /= scalar;

        return *this;
    }
};

}