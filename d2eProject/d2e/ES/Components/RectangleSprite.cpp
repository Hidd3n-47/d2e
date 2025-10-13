#include "d2ePch.h"
#include "RectangleSprite.h"

#include "ES/GameObject.h"
#include "Es/Components/Transform.h"

namespace d2e
{

void RectangleSprite::Render(WeakRef<sf::RenderWindow> window)
{
    const auto transform = mParent->GetComponent<Transform>();

    mRectangle.setPosition({ transform->translation.x - mHalfExtents.x, transform->translation.y - mHalfExtents.y });
    mRectangle.setSize({ mHalfExtents.x * 2.0f, mHalfExtents.y * 2.0f });
    window->draw(mRectangle);
}

} // Namespace d2e.
