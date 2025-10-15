#pragma once

#include "d2e/ES/IComponent.h"

namespace d2e
{

class Particle;

struct ParticleEffectDetails
{
    float    minSpawnAngle = 0.0f;
    float    maxSpawnAngle = 45.0f;
    float    minResistance = 1.0f;
    float    maxResistance = 1.0f;
    float    minLifespan   = 0.2f;
    float    maxLifespan   = 0.2f;
    float    minSpeed      = 10.0f;
    float    maxSpeed      = 10.0f;
    float    minRadius     = 5.0f;
    float    maxRadius     = 10.0f;
};

class ParticleEffect : public IComponent
{
public:
    //ParticleEffect(const uint32_t maxParticles = 10/*, const ParticleEffectDetails& effectDetails = {}*/);

    void EmitParticles(const uint32_t numParticles, const Vec2 startingPosition, const ParticleEffectDetails& details = {});

    void Update(const float dt) override;
    void Render(const WeakRef<sf::RenderWindow> window) override;

    D2E_COMPONENT("ParticleEffect")
private:
    uint32_t mMaxParticles;
    //ParticleEffectDetails mEffectDetails;

    std::vector<Particle> mParticles;
    uint32_t mNumAliveParticles = 0;
};

} // Namespace d2e.
