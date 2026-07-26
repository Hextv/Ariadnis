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
	
	template<typename TerrainType>
	void render(TerrainType& terrain, int colorLoc) {
		switch (currentMode) {
		case RenderModes::SOLID:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniform4f(colorLoc, 0.2f, 0.5f, 0.3f, 1.0f);
			terrain.draw();
			break;

		case RenderModes::WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.5f);
			glUniform4f(colorLoc, 0.31f, 0.62f, 0.24f, 1.0f);
			terrain.draw();
			break;

		case RenderModes::SOLID_WITH_WIREFRAME:
			// Base Mesh
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_POLYGON_OFFSET_FILL);
			glPolygonOffset(1.0f, 1.0f);
			glUniform4f(colorLoc, 0.15f, 0.20f, 0.22f, 1.0f);
			terrain.draw();
			glDisable(GL_POLYGON_OFFSET_FILL);

			// Wireframe Overlay
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(1.2f);
			glUniform4f(colorLoc, 0.31f, 0.62f, 0.24f, 1.0f);
			terrain.draw();
			break;
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
};