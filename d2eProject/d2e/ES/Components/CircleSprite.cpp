#include "d2ePch.h"
#include "CircleSprite.h"

#include "ES/GameObject.h"
#include "ES/Components/Transform.h"

namespace d2e
{

void CircleSprite::Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera)
{
    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    const Vec2 position{ camera.PositionToScreenSpace(Vec2{ transform->translation.x - mRadius, transform->translation.y + mRadius }) };
    const Vec2 size{ camera.SizeInScreenSpace(Vec2{ mRadius * 2.0f, 0.0f }) };

    mCircle.setPosition({ position.x, position.y });
    mCircle.setRadius(size.x);
    mCircle.setFillColor(mColor);
    window->draw(mCircle);
}

} // Namespace d2e.
