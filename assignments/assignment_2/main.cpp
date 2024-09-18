#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Shader File/shader.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	float vertices[] = {
		// The first triangle
		//X      Y    Z     
	   0.5f,  0.5f, 0.0f, // top right
	   0.5f, -0.5f, 0.0f, // bottom right
	  -0.5f,  0.5f, 0.0f, // top left

		// The second triangle
		0.5f, -0.5f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f, // bottom left
	   -0.5f,  0.5f, 0.0f, // top left
	};

	unsigned int indices[] =
	{
		0,1,3, // first triangle
		1,2,3  // second triangle
	};

	unsigned int EBO; 
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Initialization goes here!
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO; 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position (XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color RGBA
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);


	// creates the shader object
	shaderFile::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	int timeLocation = glGetUniformLocation(shader.ID, "uTime");


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Clear framebuffer
		glClearColor(0.0f, 0.5f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// update the time
		float time = (float)glfwGetTime();

		glUniform1f(timeLocation, time);

		shader.use();

		// DRAW CALL
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Drawing happens here
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}