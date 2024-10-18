#ifndef  CAMERA_H
#define  CAMERA_H

#include "../ew/external/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines each option for camera movement. 
enum Camera_Movement
{
	FORWARD, 
	BACKWARD,
	LEFT,
	RIGHT, 
	UP, 
	DOWN
};

// Defualt camera values 
const float YAW			=	-90.0f;
const float PITCH		=	 0.0f;
const float SPEED		=	 2.5f;
const float SENSITIVITY	=	 0.1f;
const float ZOOM		=	 45.0f;

namespace shaderFile
{
	class Camera
	{
	public:
		// Camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		// euler Angles
		float Yaw;
		float Pitch;
		// camera options
		float MovementSpeed;
		float BaseMovementSpeed;
		float MouseSensitivity;
		float Zoom;

		// constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

		// constructor with sclar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		// returs the view matrix
		glm::mat4 GetViewMatrix();

		// processes input from any keyboard like in put system
		void ProcessKeyboard(Camera_Movement direction, float deltaTime);

		// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

		// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(float yoffset);

	private:
		// calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();
	};
}
#endif // ! CAMERA_H
