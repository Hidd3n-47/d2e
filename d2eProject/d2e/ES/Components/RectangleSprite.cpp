#include "d2ePch.h"
#include "RectangleSprite.h"

#include "ES/GameObject.h"
#include "Es/Components/Transform.h"

namespace d2e
{

void RectangleSprite::Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera)
{
    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    const Vec2 position{ camera.PositionToScreenSpace(transform->translation + Vec2{ -mHalfExtents.x, mHalfExtents.y }) };
    const Vec2 size{ camera.SizeInScreenSpace(mHalfExtents * 2.0f) };

    mRectangle.setPosition({ position.x, position.y });
    mRectangle.setSize({ size.x, size.y });
    window->draw(mRectangle);
}

} // Namespace d2e.
