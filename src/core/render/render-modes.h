#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class RenderModes {
	SOLID,
	WIREFRAME,
	SOLID_WITH_WIREFRAME
};

class TerrainRenderMode {
public:
	RenderModes currentMode = RenderModes::WIREFRAME; // Current mode is set to Wireframe

	void handleInput(GLFWwindow* window) {
		static bool keyOpressed = false;

		// Logic: If "O" is pressed, then the Render Mode will change to the next one
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && !keyOpressed) {
			// Cycle
			switch (currentMode) {
			case RenderModes::SOLID:
				currentMode = RenderModes::WIREFRAME;
				break;
			case RenderModes::WIREFRAME:
				currentMode = RenderModes::SOLID_WITH_WIREFRAME;
				break;
			case RenderModes::SOLID_WITH_WIREFRAME:
				currentMode = RenderModes::SOLID;
				break;
			}
			keyOpressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) {
			keyOpressed = false;
		}
	}
	// TODO: Continue the render modes.
};