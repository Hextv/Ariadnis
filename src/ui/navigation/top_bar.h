#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

namespace UI {
	class TopBar {
	public:

		void render(GLFWwindow* window) {
			// Fixed menu bar pinned to the top of the window
			if (ImGui::BeginMainMenuBar()) {

				// "File" Menu
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("New Terrain")) {
						// TODO: Action for New Terrain
					}
					if (ImGui::MenuItem("Save")) {
						// TODO: Action for Save
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", "Esc")) {
						glfwSetWindowShouldClose(window, true);
					}
					ImGui::EndMenu(); // Close "File" menu
				}

				ImGui::EndMainMenuBar(); // Close main menu bar
			}
		}

	};
}