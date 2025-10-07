#pragma once

namespace d2e
{
class Particle
{
public:
    Particle() = default;
    Particle(const Vec2 position, const Vec2 velocity, const float resistance, const float lifetime, const float radius);

    bool Update(const float dt);

    void Render(WeakRef<sf::RenderWindow> window);
private:
    Vec2  mPosition;
    Vec2  mVelocity;
    float mResistance = 0.0f;
    float mLifetime   = 0.0f;
    float mRadius     = 10.0f;

    sf::CircleShape mParticle;
};


}
