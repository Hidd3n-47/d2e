#pragma once

namespace d2e
{

struct OrthoCameraValues
{
    float left, right, top, bottom;
    float screenWidth, screenHeight;
};

class OrthoCamera
{
public:
    OrthoCamera() = default;
    OrthoCamera(const OrthoCameraValues& values);

    [[nodiscard]] Vec2 ScreenSizeToWorldSize(const Vec2 screenSpace) const;
    [[nodiscard]] Vec2 PositionToScreenSpace(const Vec2 position) const;
    [[nodiscard]] inline Vec2 SizeInScreenSpace(const Vec2 size) const { return PositionToScreenSpace(size * Vec2{ 1.0f, -1.0f }) - mScreenCoordsOfCentre; }

    [[nodiscard]] inline Vec2 GetScreenSize() const { return Vec2{ mScreenWidth, mScreenHeight }; }
private:
    float mLeft, mRight;
    float mTop, mBottom;

    float mScreenWidth, mScreenHeight;

    Vec2 mScreenCoordsOfCentre;
};

} // Namespace d2e.