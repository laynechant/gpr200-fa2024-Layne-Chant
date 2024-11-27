#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <glm/glm.hpp>
#include <vector>
#include "../ew/external/glad.h"
#include <glm/gtc/type_ptr.hpp>

struct Particle {
    glm::vec2 Position; // Position of the particle
    glm::vec2 Velocity; // Velocity of the particle
    glm::vec4 Color;    // RGBA color of the particle
    float Lifetime;     // Remaining life of the particle in seconds

    Particle()
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Lifetime(0.0f) {}
};

// Particle system to manage and render particles
class ParticleSystem {
public:
    // Constructor
    ParticleSystem(unsigned int maxParticles);

    // Updates all particles in the system
    void Update(float deltaTime);

    // Renders all active particles
    void Render();

    // Spawns a new particle at a given position with initial velocity and color
    void SpawnParticle(const glm::vec2& position,
        const glm::vec2& velocity,
        const glm::vec4& color,
        float lifetime);

private:
    std::vector<Particle> Particles; // Container for all particles
    unsigned int MaxParticles;       // Maximum number of particles allowed
};

#endif PARTICLESYSTEM_H