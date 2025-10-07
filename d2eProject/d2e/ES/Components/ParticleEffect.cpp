#include "d2ePch.h"
#include "ParticleEffect.h"

#include "Math/Math.h"
#include "Core/Random.h"
#include "ParticleSystem/Particle.h"

namespace d2e
{

ParticleEffect::ParticleEffect(const uint32_t maxParticles/*, const ParticleEffectDetails& effectDetails*/)
    : mMaxParticles(maxParticles)
    //, mEffectDetails(effectDetails)
{
    mParticles.resize(mMaxParticles);
}

void ParticleEffect::EmitParticles(const uint32_t numParticles, const Vec2 startingPosition, const ParticleEffectDetails& details)
{
    for (uint32_t i{ 0 }; i < numParticles; ++i)
    {
        const uint32_t index = mNumAliveParticles++ % mMaxParticles;

        const float spawnAngle = Random::GetRandomBetween<float>(details.minSpawnAngle, details.maxSpawnAngle);
        const float resistance = Random::GetRandomBetween<float>(details.minResistance, details.maxResistance);
        const float lifetime   = Random::GetRandomBetween<float>(details.minLifespan, details.maxLifespan);
        const float speed      = Random::GetRandomBetween<float>(details.minSpeed, details.maxSpeed);
        const float radius     = Random::GetRandomBetween<float>(details.minRadius, details.maxRadius);

        const float cosAngle = std::cos(Math::DegToRads(spawnAngle));
        const float sinAngle = std::sin(Math::DegToRads(spawnAngle));
        const Vec2 velocity  = Vec2{ cosAngle, sinAngle } * speed;

        mParticles[index] = Particle{ startingPosition, velocity, resistance, lifetime, radius };
    }
}

void ParticleEffect::Update(const float dt)
{
    for (uint32_t i{ 0 }; i < mNumAliveParticles; ++i)
    {
        if (!mParticles[i].Update(dt))
        {
            if (i == mNumAliveParticles)
            {
                --mNumAliveParticles;
                continue;
            }

            mParticles[i] = mParticles[--mNumAliveParticles];

            --i;
        }
    }
}

void ParticleEffect::Render(const WeakRef<sf::RenderWindow> window)
{
    for (uint32_t i{ 0 }; i < mNumAliveParticles; ++i)
    {
        mParticles[i].Render(window);
    }
}

} // Namespace d2e.
