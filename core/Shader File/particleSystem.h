#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <iostream>
#include "shader.h"

enum class ParticleType {
    SNOW,
    FIRE,
    SMOKE
};

class ParticleSystem {
public:
    struct Particle {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 velocity = glm::vec3(1.0f);
        glm::vec2 texCoord;
        float lifetime;
        ParticleType type;
        unsigned int textureID; // Holds the texture ID for this particle
    };

    std::vector<Particle> particles;
    unsigned int maxParticles;
    unsigned int VAO, VBO, EBO;
    shaderFile::Shader& particleShader;
    glm::vec3 emitterPosition;

    // Textures for different particle types
    unsigned int snowTextureID, fireTextureID, smokeTextureID;

    ParticleSystem(int maxParticles, shaderFile::Shader& shader, glm::vec3 emitterPosition,
        unsigned int snowTexture/*, unsigned int fireTexture, unsigned int smokeTexture*/)
        : maxParticles(maxParticles), particleShader(shader), emitterPosition(emitterPosition),
        snowTextureID(snowTexture)/*, fireTextureID(fireTexture), smokeTextureID(smokeTexture)*/ {
        particles.reserve(maxParticles);
        initializeBuffers();
        resetParticles();
        std::cout << "Current number of particles: " << particles.size() << std::endl;
    }

    void resetParticles() {
        std::cout << "Resetting particles..." << std::endl;
        for (auto& particle : particles) {
            resetParticle(particle, randomParticleType());
        }
    }

    void resetParticle(Particle& particle, ParticleType type) {
        particle.position = emitterPosition;
        particle.type = type;

        switch (type) {
        case ParticleType::SNOW:
            particle.velocity = glm::vec3(randFloat(-0.2f, 0.2f), randFloat(-0.5f, -0.2f), randFloat(-0.2f, 0.2f));
            particle.color = glm::vec3(1.0f, 1.0f, 1.0f);
            particle.lifetime = randFloat(3.0f, 7.0f);
            particle.textureID = snowTextureID;
            break;

        case ParticleType::FIRE:
            particle.velocity = glm::vec3(randFloat(-0.1f, 0.1f), randFloat(0.5f, 1.5f), randFloat(-0.1f, 0.1f));
            particle.color = glm::vec3(randFloat(0.8f, 1.0f), randFloat(0.3f, 0.6f), 0.0f);
            particle.lifetime = randFloat(1.0f, 3.0f);
            //particle.textureID = fireTextureID;
            break;

        case ParticleType::SMOKE:
            particle.velocity = glm::vec3(randFloat(-0.3f, 0.3f), randFloat(0.1f, 0.5f), randFloat(-0.3f, 0.3f));
            particle.color = glm::vec3(0.5f, 0.5f, 0.5f);
            particle.lifetime = randFloat(2.0f, 5.0f);
            //particle.textureID = smokeTextureID;
            break;
        }

        particle.texCoord = glm::vec2(0.0f, 0.0f); // Default texCoord
    }

    void update(float deltaTime) {
        for (auto& particle : particles) {
            particle.lifetime -= deltaTime;
            if (particle.lifetime <= 0.0f) {
                resetParticle(particle, randomParticleType()); // Reset with random type
            }
            else {
                particle.position += particle.velocity * deltaTime; // Update position
            }
        }
    }

    void emitParticle(ParticleType type) {
        if (particles.size() < maxParticles) {
            Particle newParticle;
            newParticle.position = emitterPosition;
            newParticle.velocity = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(0.5f, 1.5f), randFloat(-1.0f, 1.0f));
            newParticle.lifetime = randFloat(2.0f, 5.0f); // Random lifetime
            newParticle.type = type; // Assign the type
            newParticle.color = getDefaultColorForType(type); // Assign a color based on the type
            particles.push_back(newParticle);
        }
    }

    glm::vec3 getDefaultColorForType(ParticleType type) {
        switch (type) {
        case ParticleType::SNOW:
            return glm::vec3(1.0f, 1.0f, 1.0f); // White
        case ParticleType::FIRE:
            return glm::vec3(1.0f, 0.5f, 0.0f); // Orange
        case ParticleType::SMOKE:
            return glm::vec3(0.5f, 0.5f, 0.5f); // Gray
        default:
            return glm::vec3(1.0f, 1.0f, 1.0f); // Default white
        }
    }

    void render(const glm::mat4& view, const glm::mat4& projection) {
        if (particles.empty()) return; // No particles to render

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        particleShader.use();
        particleShader.setMat4("view", view);
        particleShader.setMat4("projection", projection);

        for (const auto& particle : particles) {
            glBindTexture(GL_TEXTURE_2D, particle.textureID); // Bind the texture for this particle

            glm::mat4 model = glm::translate(glm::mat4(1.0f), particle.position);
            model = glm::scale(model, glm::vec3(0.1f)); // Example scale
            particleShader.setMat4("model", model);
            particleShader.setVec3("particleColor", particle.color);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Render quad
        }

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }

private:
    void initializeBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        float vertices[] = {
            // Positions        // Texture Coords
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2, 0, 3, 2
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    ParticleType randomParticleType() {
        static std::mt19937 gen{ std::random_device{}() };
        std::uniform_int_distribution<int> dist(0, 2);
        return static_cast<ParticleType>(dist(gen));
    }

    float randFloat(float min, float max) {
        static std::mt19937 gen{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }
};

#endif // PARTICLE_SYSTEM_H
