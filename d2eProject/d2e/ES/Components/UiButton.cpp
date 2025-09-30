#include "d2ePch.h"
#include "UiButton.h"

#include <d2e/ES/GameObject.h>

namespace d2e
{

void UiButton::OnComponentAdded(const WeakRef<GameObject> parent)
{
    IComponent::OnComponentAdded(parent);
}

void UiButton::Update(const float dt)
{
    //mParent->GetComponent<Transform>()->translation = mParent->GetComponent<Transform>()->translation + Vec2{ 10.f * dt, 10.0f * dt };
}

void UiButton::Render(WeakRef<sf::RenderWindow> window)
{
    const auto transform = mParent->GetComponent<Transform>();

    mRectangle.setPosition({ transform->translation.x, transform->translation.y });
    mRectangle.setSize({ mSize.x, mSize.y });
    window->draw(mRectangle);
}

} // Namespace d2e.
