#include "camera.h"

// Constructor with default values
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: CameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(2.5f),
	MouseSensitivity(0.1f),
	Zoom(45.0f)
{
	CameraPos = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

// Generates the LookAt View Matrix for OpenGL shaders
glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

// Processes input received from keyboard-like movement keys
void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;

	if (direction == CameraMovement::FORWARD)
		CameraPos += CameraFront * velocity;
	if (direction == CameraMovement::BACKWARD)
		CameraPos -= CameraFront * velocity;
	if (direction == CameraMovement::LEFT)
		CameraPos -= CameraRight * velocity;
	if (direction == CameraMovement::RIGHT)
		CameraPos += CameraRight * velocity;
	if (direction == CameraMovement::UP)
		CameraPos += WorldUp * velocity;
	if (direction == CameraMovement::DOWN)
		CameraPos -= WorldUp * velocity;
}

// Processes input received from mouse cursor movement
void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update CameraFront, CameraRight and CameraUp Vectors using the updated Euler angles
	updateCameraVectors();
}

// Processes input received from mouse scroll wheel (Zoom / FOV)
void Camera::processMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	CameraFront = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	// Normalize the vectors because their length gets closer to 0 the more you look up or down
	CameraRight = glm::normalize(glm::cross(CameraFront, WorldUp));
	CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));
}