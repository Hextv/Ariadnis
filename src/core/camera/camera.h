#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>

#pragma once

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
	glm::vec3 CemeraUp;
	glm::vec3 CameraRight;
	glm::vec3 WorldUp;

	// Angles
	float Yaw;			// Left and Right
	float Pitch;	   // Up and Down 

	// Options
	float MouseSesitivity;
	float MovementSpeed;
	float Zoom;

	void processKeyboard(CameraMovement direction, float deltaTime);
};