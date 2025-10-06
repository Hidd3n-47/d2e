#include "d2ePch.h"
#include "Animation.h"

#include "ES/GameObject.h"

namespace d2e
{

Animation::Animation()
    : mTexture("E:/Programming/d2e/d2eGameProject/d2eGame/Assets/InvalidTexture.png")
    , mVisual(mTexture)
    , mSpriteColor(sf::Color::White)
{
    // Empty.
}

void Animation::CreateAnimation(const std::string& filepath, const AnimationDetails details, const float timeBetweenFrames)
{
    mAnimationDetails  = details;
    mTimeBetweenFrames = timeBetweenFrames;

    mTexture = sf::Texture{ filepath };
    mVisual  = sf::Sprite{ mTexture };
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

    mVisual.setScale({ transform->scale.x, transform->scale.y });
    mVisual.setPosition({ transform->translation.x - 0.5f * mAnimationDetails.textureSize.x * transform->scale.x, transform->translation.y - 0.5f * mAnimationDetails.textureSize.y * transform->scale.y });
    mVisual.setColor(mSpriteColor);

    const sf::Vector2i size = { static_cast<int>(mAnimationDetails.textureSize.x), static_cast<int>(mAnimationDetails.textureSize.y) };

    const sf::Vector2i topLeft =
    {
        static_cast<int>((mCurrentFrame % mAnimationDetails.framesHorizontal) * size.x),
        static_cast<int>((mCurrentFrame / mAnimationDetails.framesHorizontal) * size.y)
    };

    mVisual.setTextureRect(sf::IntRect { topLeft, size });
    window->draw(mVisual);
}

} // Namespace d2e.
