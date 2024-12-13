#version 330 core

layout (location = 0) in vec3 particleLocation; // Particle local position
layout (location = 1) in vec4 particlePositionSize; // Position and size of the particle
layout (location = 2) in vec4 particleColor; // Particle color

uniform mat4 view; // Camera view matrix
uniform mat4 projection; // Camera projection matrix
uniform vec3 cameraPosition; // Camera position for billboard effect

out vec4 fragColor;
out vec2 texCoords; // Texture coordinates

void main() {
    // Calculate the position of the particle in world space
    vec3 pos = particleLocation * particlePositionSize.w + particlePositionSize.xyz;

    // Calculate the direction vector from the particle to the camera
    vec3 toCamera = normalize(cameraPosition - pos);

    // Create a rotation matrix to align the particle with the camera
    mat3 rotationMatrix = mat3(
        normalize(vec3(view[0][0], view[1][0], view[2][0])), // Right vector of the camera
        normalize(vec3(view[0][1], view[1][1], view[2][1])), // Up vector of the camera
        toCamera // Forward vector to the camera
    );

    // Rotate the particle based on the camera's orientation
    vec3 rotatedPos = rotationMatrix * particleLocation;

    // Update the position to the camera-facing position
    pos = rotatedPos * particlePositionSize.w + particlePositionSize.xyz;

    // Project the position into clip space
    gl_Position = projection * view * vec4(pos, 1.0);

    // Set the texture coordinates for a quad (normalized to [0, 1] range)
    texCoords = vec2(0.5, 0.5); // bottom-left corner of the texture
    fragColor = particleColor;
}