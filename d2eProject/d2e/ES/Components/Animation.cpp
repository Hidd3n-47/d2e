#include "d2ePch.h"
#include "Animation.h"

#include "ES/GameObject.h"

namespace d2e
{

Animation::Animation()
    : mSpriteColor(sf::Color::White)
{
    // Empty.
}

void Animation::CreateAnimation(const AnimationDetails& details, const float timeBetweenFrames)
{
    mAnimationDetails  = details;
    mTimeBetweenFrames = timeBetweenFrames;
}

void Animation::Update(const float dt)
{
    mTimer += dt;
    // todo look at making this condition nicer :)
    if (mTimer >= mTimeBetweenFrames && (mAnimationDetails.repeatAnimation || (!mAnimationDetails.repeatAnimation && mCurrentFrame < mAnimationDetails.frameCount - 1)))
    {
        mCurrentFrame = (mCurrentFrame + 1) % mAnimationDetails.frameCount;
        mTimer = 0.0f;
    }
}

void Animation::Render(WeakRef<sf::RenderWindow> window)
{
    const WeakRef<Transform> transform = mParent->GetComponent<Transform>();

    sf::Sprite visual = SpriteManager::Instance()->GetSprite(mAnimationDetails.spriteSheetId)->sprite;

    visual.setScale({ transform->scale.x, transform->scale.y });
    visual.setPosition({ transform->translation.x - 0.5f * mAnimationDetails.textureSize.x * transform->scale.x, transform->translation.y - 0.5f * mAnimationDetails.textureSize.y * transform->scale.y });
    visual.setColor(mSpriteColor);

    const sf::Vector2i size = { static_cast<int>(mAnimationDetails.textureSize.x), static_cast<int>(mAnimationDetails.textureSize.y) };

    const sf::Vector2i topLeft =
    {
        static_cast<int>((mCurrentFrame % mAnimationDetails.framesHorizontal) * size.x),
        static_cast<int>((mCurrentFrame / mAnimationDetails.framesHorizontal) * size.y)
    };

    visual.setTextureRect(sf::IntRect { topLeft, size });
    window->draw(visual);
}

} // Namespace d2e.
