#include "d2ePch.h"
#include "OrthoCamera.h"

namespace d2e
{

OrthoCamera::OrthoCamera(const OrthoCameraValues& values)
    : mLeft(values.left)
    , mRight(values.right)
    , mTop(values.top)
    , mBottom(values.bottom)
    , mScreenWidth(values.screenWidth)
    , mScreenHeight(values.screenHeight)
    , mScreenCoordsOfCentre{ mScreenWidth * 0.5f, mScreenHeight * 0.5f }
{
    // Empty.
}

Vec2 OrthoCamera::ScreenSizeToWorldSize(const Vec2 screenSpace) const
{
    Vec2 worldSpace = screenSpace / Vec2{ mScreenWidth, mScreenHeight };
    worldSpace *= 2.0f;

    worldSpace *= Vec2{ mRight, mTop };

    return worldSpace;
}

Vec2 OrthoCamera::PositionToScreenSpace(const Vec2 position) const
{
    Vec2 screenspace = position / Vec2{ (mRight - mLeft) * 0.5f, (mBottom - mTop) * 0.5f };
    screenspace += Vec2{ 1.0f };
    screenspace /= 2.0f;
    screenspace *= Vec2{ mScreenWidth, mScreenHeight };

    return screenspace;
}


} // Namespace d2e.