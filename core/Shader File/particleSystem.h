#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <iostream>
#include "shader.h"

class ParticleSystem {
public:
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity = glm::vec3(1.0f);
        glm::vec2 texCoord;
        float lifetime = 10000.0f;
    };

    std::vector<Particle> particles;
    unsigned int maxParticles;
    unsigned int VAO, VBO, EBO, textureID;
    shaderFile::Shader& particleShader;
    glm::vec3 emitterPosition;

    ParticleSystem(int maxParticles, shaderFile::Shader& shader, glm::vec3 emitterPosition, unsigned int textureID)
        : maxParticles(maxParticles), particleShader(shader), emitterPosition(emitterPosition), textureID(textureID) {
        particles.reserve(maxParticles);
        initializeBuffers();
        resetParticles();
        std::cout << "Current number of particles: " << particles.size() << std::endl;
    }

    void resetParticles() {
        std::cout << "Resetting particles..." << std::endl;
        for (auto& particle : particles) {
            resetParticle(particle);
        }
    }

    void resetParticle(Particle& particle) {
        particle.position = emitterPosition;
        particle.velocity = glm::vec3(randFloat(-1.0f, 1.0f), randFloat(0.5f, 1.5f), randFloat(-1.0f, 1.0f));
        particle.lifetime = randFloat(2.0f, 5.0f);  // Random lifetime between 2 and 5 seconds
        particle.texCoord = glm::vec2(0.0f, 0.0f);  // Set a default texture coordinate
        std::cout << "Particle reset: Position = " << particle.position.x << ", " << particle.position.y << ", " << particle.position.z
            << ", Lifetime = " << particle.lifetime << std::endl;
    }

    void update(float deltaTime) {
        for (auto& particle : particles) {
            particle.lifetime -= deltaTime;
            if (particle.lifetime <= 0.0f) {
                resetParticle(particle);  // Reset particle if it has expired
            }
            else {
                particle.position += particle.velocity * deltaTime;  // Update position with velocity
            }
        }
    }

    void EmitParticle() {
        if (particles.size() < maxParticles) {
            Particle newParticle;
            newParticle.position = emitterPosition;
            newParticle.velocity = glm::vec3(randFloat(-1.0f, 1.0f), -1.0f, 0.0f);
            newParticle.lifetime = 10000;  // Set a fixed lifetime for the emitted particle
            particles.push_back(newParticle);
        }
    }

    void render(const glm::mat4& view, const glm::mat4& projection) {
        if (particles.empty()) {
            return;  // No particles to render
        }

        // Set up the transformations for each particle
        std::vector<glm::mat4> particleTransforms;
        for (const auto& particle : particles) {
            // Set up transformation matrix for each particle
            glm::mat4 model = glm::translate(glm::mat4(1.0f), particle.position);
            // You can scale or rotate the particle based on lifetime or any other factor
            model = glm::scale(model, glm::vec3(0.1f)); // Example scale

            // Store the transformation for each particle
            particleTransforms.push_back(model);
        }

        // Bind texture for all particles
   /*     glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);*/

        // Disable depth testing to ensure particles render on top of everything
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        particleShader.use();
        particleShader.setMat4("view", view);
        particleShader.setMat4("projection", projection);

        // Now render each particle as a quad
        for (size_t i = 0; i < particles.size(); ++i) {
            // Update the model matrix for each particle
            particleShader.setMat4("model", particleTransforms[i]);
            particleShader.setInt("particleTexture", 0);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // Render quad for each particle
        }

        // Re-enable depth testing and disable blending after rendering particles
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }


private:
    void initializeBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Define the quad vertices and indices (two triangles)
        float vertices[] = {
            // Positions        // Texture Coords
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
            0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,  0, 3, 2
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

    float randFloat(float min, float max) {
        static std::mt19937 gen{ std::random_device{}() };
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }
};


#endif // PARTICLE_SYSTEM_H
