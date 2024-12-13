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
#include <Shader File/cubemap.h>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

shaderFile::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main() {
    printf("Initializing...\n");

    // Initialize GLFW
    if (!glfwInit()) {
        printf("GLFW failed to init!\n");
        return 1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Final Project", NULL, NULL);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Enable depth testing    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Create a shader program
    shaderFile::Shader brickShader("assets/vertexShader.vert", "assets/fragmentShader.frag");
    shaderFile::Shader lightShader("assets/vertexShaderLight.vert", "assets/fragmentShaderLight.frag");

    // Load the shaders for the object
    shaderFile::Shader ourShader("assets/modelLoading.vert", "assets/modelLoading.frag");
    shaderFile::Shader particleShader("assets/particle.vert", "assets/particle.frag");
    shaderFile::Shader skyBoxShader("assets/cubeMapShader.vert", "assets/cubeMapShader.frag");

    // Make sure the model isnt upside down
    stbi_set_flip_vertically_on_load(true);

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    vector<std::string> faces
    {
        "assets/skybox/posx.jpg",
        "assets/skybox/negx.jpg",
        "assets/skybox/posy.jpg",
        "assets/skybox/negy.jpg",
        "assets/skybox/posz.jpg",
        "assets/skybox/negz.jpg"
    };
    ShaderFile::Cubemap cubeMapTexture = ShaderFile::Cubemap(faces);


    // Load the object
    ShaderFile::Model cabinModel("assets/Cabin.obj");
    shaderFile::Texture2D cabinTexture("assets/TexturesCom_WoodLogs0019_4_XL.jpg", GL_LINEAR, GL_CLAMP_TO_EDGE, false);

	shaderFile::Texture2D particleTexture("assets/lightcookie.jpg", GL_LINEAR, GL_CLAMP_TO_EDGE, true);
    
	ParticleSystem particleSystem(particleShader, particleTexture.GetID());


    // Define the projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    float ambientStrength = 0.1f;

    float diffStrength = 1.0f;
    float specularStrength = 0.5f;
    float shininess = 512.0f;

    // adjusts the rotation and position of the cabin
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 position = glm::vec3(-4.0f, -5.5f, -15.0f);
    model = glm::translate(model, position);

    float angle = glm::radians(88.0f);
    glm::vec3 axis = glm::vec3(0.0f, 6.0f, 0.0f); // rotates around the x axis

    model = glm::rotate(model, angle, axis);

    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


	particleSystem.CreateSnowEmitters(particleSystem, 10, 10, 2.0f);
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

        // Set up the view/projection matrices
        glm::mat4 view = camera.GetViewMatrix();

        // Render the other objects
        //particleSystem.emitParticle(ParticleType::SNOW);

        skyBoxShader.use();
        glDepthMask(GL_FALSE);
        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyBoxShader.setMat4("view", skyboxView);
        skyBoxShader.setMat4("projection", projection);

        // set view and projection matrix for the cube map
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture.GetID());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glDepthMask(GL_TRUE);

        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cabinTexture.GetID());
        ourShader.setInt("texture_diffuse", 0);

        cabinModel.draw(ourShader);
        glDepthMask(GL_FALSE);

        particleSystem.Update(deltaTime, camera.Position);
        // Render the particles
        particleSystem.Render(view, projection, camera.Position);

        glDepthFunc(GL_LESS);   // Restore standard depth function
        glDepthMask(GL_TRUE);   // Re-enable depth writing

            //// Render the GUI
            //ImGui_ImplGlfw_NewFrame();
            //ImGui_ImplOpenGL3_NewFrame();
            //ImGui::NewFrame();


            glfwSwapBuffers(window);
            glfwPollEvents();
        }
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
    void framebuffer_size_callback(GLFWwindow * window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    // glfw: whenever the mouse moves, this callback is called
    void mouse_callback(GLFWwindow * window, double xposIn, double yposIn)
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
    void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
    {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }

