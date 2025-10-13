#include "d2ePch.h"
#include "CircleSprite.h"

#include "ES/GameObject.h"
#include "ES/Components/Transform.h"

namespace d2e
{

void CircleSprite::Render(WeakRef<sf::RenderWindow> window)
{
    const auto transform = mParent->GetComponent<Transform>();

    mCircle.setPosition({ transform->translation.x - mRadius, transform->translation.y -mRadius });
    mCircle.setRadius(mRadius);
    window->draw(mCircle);
}

} // Namespace d2e.
