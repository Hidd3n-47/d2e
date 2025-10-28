#include "d2ePch.h"
#include "Text.h"

#include "ES/GameObject.h"
#include "ES/Components/Transform.h"

namespace d2e
{

void Text::Render(WeakRef<sf::RenderWindow> window, const OrthoCamera& camera)
{
    if (!mRender)
    {
        return;
    }

    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    const Vec2 position = camera.PositionToScreenSpace(transform->translation);
    sf::FloatRect bounds = mText.getLocalBounds();

    mText.setPosition({ position.x - bounds.size.x * 0.5f, position.y - bounds.size.y });
    mText.setCharacterSize(mTextSize);

    window->draw(mText);
}

void Text::SetText(const std::string& text)
{
    mTextString = text;
    mText.setString(mTextString);
}

} // Namespace d2e.