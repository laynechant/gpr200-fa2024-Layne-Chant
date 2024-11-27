#include "particleSystem.h"


// Constructor
ParticleSystem::ParticleSystem(unsigned int maxParticles)
    : MaxParticles(maxParticles) {
    Particles.reserve(maxParticles);
    for (unsigned int i = 0; i < maxParticles; ++i) {
        Particles.emplace_back(); // Initialize all particles
    }
}

// Update all particles
void ParticleSystem::Update(float deltaTime) {
    for (Particle& particle : Particles) {
        if (particle.Lifetime > 0.0f) {
            // Decrease lifetime
            particle.Lifetime -= deltaTime;

            // Update position based on velocity
            particle.Position += particle.Velocity * deltaTime;

            // Optionally, fade the particle over time
            float lifeRatio = particle.Lifetime / 5.0f; // Assuming max lifetime is 5 seconds
            particle.Color.a = glm::max(lifeRatio, 0.0f); // Reduce alpha over time
        }
    }
}

// Render all active particles
void ParticleSystem::Render() {
    // Use OpenGL to render particles
    // For simplicity, we'll assume you're rendering points (or small quads) with colors
    glBegin(GL_POINTS); // Use GL_QUADS for textured quads if needed

    for (const Particle& particle : Particles) {
        if (particle.Lifetime > 0.0f) {
            // Set color and position of the particle
            glColor4f(particle.Color.r, particle.Color.g, particle.Color.b, particle.Color.a);
            glVertex2f(particle.Position.x, particle.Position.y);
        }
    }

    glEnd();
}

// Spawn a new particle
void ParticleSystem::SpawnParticle(const glm::vec2& position,
    const glm::vec2& velocity,
    const glm::vec4& color,
    float lifetime) {
    for (Particle& particle : Particles) {
        if (particle.Lifetime <= 0.0f) {
            // Found an available particle, initialize it
            particle.Position = position;
            particle.Velocity = velocity;
            particle.Color = color;
            particle.Lifetime = lifetime;
            break; // Stop searching
        }
    }
}
