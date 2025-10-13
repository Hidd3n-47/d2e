#pragma once

#include "d2e/Core/Rtti.h"
#include "d2e/ES/IComponent.h"

#include "d2e/Rendering/SpriteManager.h"

namespace d2e
{

struct AnimationDetails
{
    spriteId spriteSheetId    = 0;
    uint32_t framesHorizontal = 1;
    uint32_t frameCount       = 1;
    Vec2     textureSize{ 512.0f, 512.0f };
    bool     repeatAnimation = false;
};

class Animation : public IComponent
{
public:
    void CreateAnimation(const AnimationDetails& details, const float timeBetweenFrames);

    void Update(const float dt) override;
    void Render(WeakRef<sf::RenderWindow> window) override;

    inline void SetSpriteColor(const sf::Color color) { mSpriteColor = color; }

    D2E_COMPONENT(Animation)
private:
    AnimationDetails mAnimationDetails{ };
    uint32_t         mCurrentFrame{ 0 };

    float mTimeBetweenFrames{ 0.2f };
    float mTimer{ 0.0f };

    sf::Color mSpriteColor = sf::Color::White;
};

} // Namespace d2e.