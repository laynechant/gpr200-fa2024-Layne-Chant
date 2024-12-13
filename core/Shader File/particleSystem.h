
#include <vector>
#include <string>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "texture2D.h"

const int MAX_PARTICLES = 10000;

struct ParticleType {
    std::string name;       // Identifier for the type
    glm::vec3 baseColor;    // Default color for particles
    float size;             // Default size of the particles
    float lifetime;         // Default lifetime
    glm::vec3 velocity;     // Default velocity
    bool useGravity;        // Whether gravity affects this particle type
};

struct Emitter {
    glm::vec3 position;    // Position of the emitter
    float rate;            // Rate of emission
    ParticleType particleType;  // The particle type this emitter will create
    float accumulator;     // Accumulator for the emitter
};

struct Particle {
    glm::vec3 position;    // Position (x, y, z)
    glm::vec3 velocity;    // Velocity (x, y, z)
    unsigned char r, g, b, a;  // Color (RGBA)
    float size;            // Particle size
    float life;            // Life time
    float cameradistance;  // Distance to camera

    bool operator<(const Particle& that) const {
        return this->cameradistance > that.cameradistance;
    }
};

class ParticleSystem {
public:
    ParticleSystem(shaderFile::Shader& shader, unsigned int textureID)
        : particleShader(shader), particleTexID(textureID),
        g_particule_position_size_data(MAX_PARTICLES * 4),
        g_particule_color_data(MAX_PARTICLES * 4),
        LastUsedParticle(0), ParticlesCount(0) {
        CreateBuffers();
        VerifyShaderAttributes();
    }

    // Store the emitters
    std::vector<Emitter> emitters;

    // Method to add emitters
    void AddEmitter(const glm::vec3& position, float rate, const ParticleType& particleType) {
        emitters.push_back(Emitter{ position, rate, particleType });
    }

    // Update function
    void ParticleSystem::Update(float deltaTime, const glm::vec3& CameraPosition) {
        for (auto& emitter : emitters) {
            // Accumulate time for the emitter
            emitter.accumulator += deltaTime;

            // Emit particles at a continuous rate
            // Keep track of how many particles should be emitted based on the rate and elapsed time
            int particlesToEmit = static_cast<int>(emitter.accumulator);  // Use the accumulated time directly
            emitter.accumulator -= static_cast<float>(particlesToEmit);  // Subtract the emitted time

            // Emit the calculated number of particles
            CreateParticles(particlesToEmit, deltaTime, emitter);
        }

        // Simulate particle behavior (movement, life, etc.)
        SimulateParticles(deltaTime, CameraPosition);

        // Sort particles based on camera distance for correct rendering order
        SortParticles();
    }
    ParticleType fireParticleType = {
        "Fire",                      // Name
        glm::vec3(1.0f, 0.5f, 0.0f), // Color (orange)
        0.1f,                         // Size
        2.0f,                         // Lifetime
        glm::vec3(0.0f, 1.0f, 0.0f), // Initial velocity
        true                          // Gravity affects this particle
    };

    ParticleType snowParticleType = {
    "Snow",                             // Name
    glm::vec3(1.0f, 1.0f, 1.0f),        // Base color (white)
    0.1f,                               // Size (smaller particles for snow)
    5.0f,                               // Lifetime
    glm::vec3(0.0f, -0.1f, 0.0f),       // Velocity (falling slowly)
    false                                // No gravity
    };
    void CreateSnowEmitters(ParticleSystem& particleSystem, int rows, int cols, float spacing) {
        // Create emitters in a grid pattern above the camera
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                // Position each emitter with a small offset in X and Z directions
                glm::vec3 emitterPosition(
                    (i - rows / 2.0f) * spacing, // Spread in the X direction
                    15.0f,                       // Fixed height above the camera
                    (j - cols / 2.0f) * spacing  // Spread in the Z direction
                );
                particleSystem.AddEmitter(emitterPosition, 300.0f, snowParticleType); // Emission rate can be adjusted as needed
            }
        }
    }


    void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix, glm::vec3& CameraPosition) {
        particleShader.use();
        particleShader.setMat4("view", viewMatrix);
        particleShader.setMat4("projection", projectionMatrix);
        particleShader.setVec3("cameraPosition", CameraPosition);

        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        glBindTextureUnit(0, particleTexID);
        particleShader.setInt("particleTexture", 0);

        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data.data());

        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data.data());

        glBindVertexArray(particleVAO);
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

private:
    Particle ParticlesContainer[MAX_PARTICLES];

    int ParticlesCount;
    shaderFile::Shader particleShader;
    unsigned int particleTexID;
    GLuint particleVAO, billboard_vertex_buffer, particles_position_buffer, particles_color_buffer;
    std::vector<GLfloat> g_particule_position_size_data;
    std::vector<GLubyte> g_particule_color_data;

    int LastUsedParticle;

    void CreateBuffers() {
        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
        };

        glGenVertexArrays(1, &particleVAO);
        glGenBuffers(1, &billboard_vertex_buffer);
        glGenBuffers(1, &particles_position_buffer);
        glGenBuffers(1, &particles_color_buffer);

        glBindVertexArray(particleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glVertexAttribDivisor(0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glVertexAttribDivisor(1, 1);

        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
        glVertexAttribDivisor(2, 1);

        glBindVertexArray(0);
    }

    void VerifyShaderAttributes() {
        particleShader.use();
        GLint posAttrib = glGetAttribLocation(particleShader.ID, "particleLocation");
        if (posAttrib == -1) {
            std::cerr << "Failed to get attribute 'particleLocation' in shader!" << std::endl;
        }

        GLint positionSizeAttrib = glGetAttribLocation(particleShader.ID, "particlePositionSize");
        if (positionSizeAttrib == -1) {
            std::cerr << "Failed to get attribute 'particlePositionSize' in shader!" << std::endl;
        }

        GLint colorAttrib = glGetAttribLocation(particleShader.ID, "particleColor");
        if (colorAttrib == -1) {
            std::cerr << "Failed to get attribute 'particleColor' in shader!" << std::endl;
        }
    }

    int FindUnusedParticle() {
        for (int i = 0; i < MAX_PARTICLES; i++) {
            int index = (LastUsedParticle + i) % MAX_PARTICLES;
            if (ParticlesContainer[index].life <= 0.0f) {
                LastUsedParticle = index;
                return index;
            }
        }
        return -1; // All particles are in use
    }


    void SortParticles() {
        std::sort(&ParticlesContainer[0], &ParticlesContainer[MAX_PARTICLES]);
    }

    void CreateParticles(int newparticles, float deltaTime, const Emitter& emitter) {
        for (int i = 0; i < newparticles; i++) {
            int particleIndex = FindUnusedParticle();
            if (particleIndex == -1) return;

            Particle& p = ParticlesContainer[particleIndex];

            // Directly use the emitter's position and particle type
            p.position = emitter.position + glm::vec3(
                ((rand() % 100) - 50) / 10.0f,  // Spread particles around emitter
                ((rand() % 100) - 50) / 10.0f,
                ((rand() % 100) - 50) / 10.0f
            );

            const ParticleType& particleType = emitter.particleType;
            p.velocity = particleType.velocity;

            // Set the color from baseColor
            p.r = static_cast<unsigned char>(particleType.baseColor.r * 255);
            p.g = static_cast<unsigned char>(particleType.baseColor.g * 255);
            p.b = static_cast<unsigned char>(particleType.baseColor.b * 255);
            p.a = 255;

            p.life = particleType.lifetime;
            p.size = particleType.size;
        }
    }



    void ParticleSystem::SimulateParticles(float deltaTime, const glm::vec3& CameraPosition) {
        ParticlesCount = 0;
        for (int i = 0; i < MAX_PARTICLES; i++) {
            Particle& p = ParticlesContainer[i];
            if (p.life > 0.0f) {
                if (p.size > 0.5f && p.velocity.y > 0.0f) {
                    p.velocity += (p.size > 0.5f ? glm::vec3(0.0f, -9.81f, 0.0f) : glm::vec3(0.0f)); // Apply gravity for fire
                }

                // Snow particles fall slower, fire particles rise
                if (p.r == 255 && p.g == 255 && p.b == 255) { // Snow
                    p.velocity.y -= 0.02f;  // Snow falls slower
                }
                else { // Fire
                    p.velocity.y += 0.1f;  // Fire rises slightly
                }

                p.position += p.velocity * deltaTime;

                p.life -= deltaTime;
                p.cameradistance = glm::length(p.position - CameraPosition);

                // Update position and color buffers
                g_particule_position_size_data[ParticlesCount * 4 + 0] = p.position.x;
                g_particule_position_size_data[ParticlesCount * 4 + 1] = p.position.y;
                g_particule_position_size_data[ParticlesCount * 4 + 2] = p.position.z;
                g_particule_position_size_data[ParticlesCount * 4 + 3] = p.size;

                g_particule_color_data[ParticlesCount * 4 + 0] = p.r;
                g_particule_color_data[ParticlesCount * 4 + 1] = p.g;
                g_particule_color_data[ParticlesCount * 4 + 2] = p.b;
                g_particule_color_data[ParticlesCount * 4 + 3] = p.a;

                ParticlesCount++;
            }
            else {
                // Reset expired particles here
                p.position = glm::vec3(0.0f);  // Reset position
                p.velocity = glm::vec3(0.0f);  // Reset velocity
                p.life = 0.0f;                 // Reset life to simulate particle death
                p.size = 0.0f;                 // Reset size
                p.r = p.g = p.b = p.a = 0;     // Reset color
            }
        }
    }

};
