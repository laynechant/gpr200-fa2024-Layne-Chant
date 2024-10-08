#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Shader File/shader.h>
#include <Shader File/texture2D.h>
#include <ew/external/stb_image.h>

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
		//postions        // colors           // texture coords
	   1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
	   1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
	  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
	  -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // top left
	};

	unsigned int indices[] =
	{
		0,1,3, // first triangle
		1,2,3  // second triangle
	};

	//Initialization goes here!
	unsigned int EBO; 
	glGenBuffers(1, &EBO);


	unsigned int VBO; 
	glGenBuffers(1, &VBO);
	// binds the Vertex array 
	
	// copys over vertices array into a vertext buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// copy the index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position (XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	shaderFile::Texture2D bg("assets/wall.jpg", GL_LINEAR, GL_REPEAT);
	unsigned int bgTexture = bg.GetID();

	shaderFile::Texture2D coin("assets/coinPerson.png", GL_NEAREST, GL_CLAMP_TO_EDGE);
	unsigned int coinPerson = coin.GetID();

	// creates the shader object
	shaderFile::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shaderFile::Shader bgShader("assets/backgroundVertexShader.vert", "assets/backgroundFragmentShader.frag");


	
	int bgTimeLocation = glGetUniformLocation(bgShader.ID, "uTime");
	int coinTimeLocation = glGetUniformLocation(shader.ID, "uTime");
	int coinTexture = glGetUniformLocation(shader.ID, "ourTexture");


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Clear framebuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bgTexture);
		

		bgShader.use();
		bgShader.setInt("uTexture", 0);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		// update the time
		float time = (float)glfwGetTime();
		glUniform1f(bgTimeLocation, time);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, coinPerson);
		

		shader.use();
		shader.setInt("ourTexture", 2);
		glUniform1f(coinTimeLocation, time);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		

		// Drawing happens here
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
