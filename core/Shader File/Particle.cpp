#include "Particle.h"

Particle::Particle(const glm::vec3& pos, const glm::vec3& vel, const glm::vec4& col,float s, float lifespan)
    : position(pos), velocity(vel), color(col), size(s), life(lifespan) {}

void Particle::update(float deltaTime) 
{
    position += velocity * deltaTime;
    life -= deltaTime;
}