#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float size;
    float life;

    Particle(const glm::vec3& pos, const glm::vec3& vel, const glm::vec4& col, float size, float life);

    void update(float deltaTime);
};

#endif PARTICLE_H