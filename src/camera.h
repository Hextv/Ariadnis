#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>

#pragma once // Prevents the header file from being included multiple times

enum class CameraDirection {
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
	// Vectors
	glm::vec3 CameraPos;

	// TODO: Need to initilize values.
	glm::vec3 CameraFront;
	glm::vec3 CemeraUp;
	glm::vec3 CameraRight;
};