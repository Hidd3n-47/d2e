#pragma once

#include <d2e/ES/IComponent.h>

namespace d2e
{

struct AnimationDetails
{
    uint32_t framesHorizontal = 1;
    uint32_t frameCount       = 1;
    Vec2     textureSize{ 512.0f, 512.0f };
    bool     repeatAnimation = false;
};

class Animation : public IComponent
{
public:
    Animation();

    void CreateAnimation(const std::string& filepath, const AnimationDetails details, const float timeBetweenFrames);

    void Update(const float dt) override;
    void Render(WeakRef<sf::RenderWindow> window) override;
private:
    AnimationDetails mAnimationDetails{};
    uint32_t         mCurrentFrame{0};

    float mTimeBetweenFrames{0.2f};
    float mTimer{0.0f};

    sf::Texture mTexture;
    sf::Sprite  mVisual;
};

} // Namespace d2e.