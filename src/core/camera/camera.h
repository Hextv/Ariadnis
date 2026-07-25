#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>

enum class CameraMovement {
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:
	// Camera Vectors
	glm::vec3 CameraPos;
	glm::vec3 CameraFront;
	glm::vec3 CameraUp;
	glm::vec3 CameraRight;
	glm::vec3 WorldUp;

	// Angles
	float Yaw;			// Left and Right
	float Pitch;	   // Up and Down 

	// Options
	float MouseSensitivity;
	float MovementSpeed;
	float Zoom;

	// Construction
	Camera(glm::vec3 position = glm::vec3(0.0f, 2.0f, 5.0f),
		glm::vec3 up = glm::vec3(0.01, 1.0f, 0.0f),
		float yaw = -90.00f,
		float pitch = 0.0f);
	
	// Matrix Calculation
	glm::mat4 GetViewMatrix() const;

	// Movement and Mouse Functions
	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void processMouseScroll(float yoffset);

private:
	// Vector Recalculation Helper
	void updateCameraVectors();
};