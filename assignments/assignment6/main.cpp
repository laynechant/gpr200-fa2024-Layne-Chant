#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <ew/external/stb_image.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Shader File/shader.h>
#include <Shader File/texture2D.h>
#include <Shader File/camera.h>
#include <Shader File/model.h>
#include <Shader File/particleSystem.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void createSnowSpawners(int amount, float height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const int MAX_PARTICLES = 500;

shaderFile::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

void createSnowSpawners(int amount, float height)
{
    //glm::vec2();


};



int main() {
    printf("Initializing...\n");

    // Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW failed to init!\n");
        return 1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
    if (window == NULL) {
        printf("GLFW failed to create window\n");
        return 1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions using glad
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers\n");
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Create a shader program
    shaderFile::Shader brickShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
    shaderFile::Shader lightShader("assets/vertexShaderLight.vert", "assets/fragmentShaderLight.frag");

    // Load the shaders for the object
    shaderFile::Shader ourShader("assets/modelLoading.vert", "assets/modelLoading.frag");
    shaderFile::Shader particleShader("assets/particleShader.vert", "assets/particleShader.frag");

    // Make sure the model isnt upside down
    stbi_set_flip_vertically_on_load(true);

   // Load the object

    ShaderFile::Model backpackModel("assets/backpack.obj");
    shaderFile::Texture2D snowflakeTexture("assets/snowflake.png", GL_LINEAR, GL_REPEAT, true);
    //snowflakeTexture.Bind();
   


    ParticleSystem particleSystem(750, particleShader, glm::vec3(0.0f, 0.0f, 0.0f), snowflakeTexture.GetID());
    // Define the projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Uniform variables
    int timeLocation = glGetUniformLocation(brickShader.ID, "uTime");
    int ambientLocation = glGetUniformLocation(brickShader.ID, "ambientStrength");
    int diffLocation = glGetUniformLocation(brickShader.ID, "diffStrength");
    int specularLocation = glGetUniformLocation(brickShader.ID, "specularStrength");
    int shininessLocation = glGetUniformLocation(brickShader.ID, "shininess");

    glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    float ambientStrength = 0.1f;

    float diffStrength = 1.0f;
    float specularStrength = 0.5f;
    float shininess = 512.0f;

    

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate the frame time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input events
        processInput(window);
        // Update the particle system
        particleSystem.update(deltaTime);

        // Set up the view/projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // Render the particles
        particleSystem.render(view, projection);

        // Render the other objects
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);
       //backpackModel.draw(ourShader);
        particleSystem.emitParticle(ParticleType::SNOW);

        // Render the GUI
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");

        ImGui::Text("Particle System");
        //ImGui::DragFloat3("Emitter Position", &particleSystem.emitterPosition.x, 0.1f);
        if (ImGui::Button("Emit Particle"))
{
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Handle sprint (SHIFT key)
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.MovementSpeed = camera.BaseMovementSpeed * 2;
    else
        camera.MovementSpeed = camera.BaseMovementSpeed;

    // Mouse cursor locking logic: hold right mouse button to lock/unlock cursor
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
        if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Lock cursor
        }
    }
    else {
        if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_NORMAL) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Unlock cursor
        }
    }

    // Handle camera movement input (W, A, S, D, Q, E)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes 
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}