#include "d2ePch.h"
#include "Particle.h"

namespace d2e
{

Particle::Particle(const Vec2 position, const Vec2 velocity, const float resistance, const float lifetime, const float radius)
    : mPosition(position)
    , mVelocity(velocity)
    , mResistance(resistance)
    , mLifetime(lifetime)
    , mRadius(radius)
{
    // Empty.
}

bool Particle::Update(const float dt)
{
    //todo does this need dt?
    mPosition += mVelocity;

    mVelocity.y -= mResistance;
    if (abs(mVelocity.y) < 0.02)
    {
        mVelocity = Vec2{};
    }

    mLifetime -= dt;

    return mLifetime > 0.0f;
}

void Particle::Render(WeakRef<sf::RenderWindow> window)
{
    if (mLifetime <= 0.0f)
    {
        return;
    }

    //DEBUG_LOG("{} {}", mPosition.x, mPosition.y);
    mParticle.setPosition({ mPosition.x, mPosition.y });
    mParticle.setRadius(mRadius);

    window->draw(mParticle);
}

} // Namespace d2e.