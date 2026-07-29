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
					if (ImGui::MenuItem("New Project")) {
						// TODO
					}
					if (ImGui::MenuItem("Open Project")) {
						// TODO
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Save")) {
						// TODO
					}
					if (ImGui::MenuItem("Save As...")) {
						// TODO
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Import...")) {
						// TODO
					}
					if (ImGui::MenuItem("Export...")) {
						// TODO
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Project Settings")) {
						// TODO
					}
					if (ImGui::MenuItem("Customization")) {
						// TODO
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", "Esc")) {
						glfwSetWindowShouldClose(window, true);
					}
					ImGui::EndMenu(); // Close "File" menu
				}

				// "Edit" Menu
				if (ImGui::BeginMenu("Edit")) {
					if (ImGui::MenuItem("Undo", "CTRL + Z")) {
						// TODO
					}
					if (ImGui::MenuItem("Redo", "CTRL + Y")) {
						// TODO
					}
					ImGui::EndMenu(); // Close "Edit" menu
				}

				// "View" Menu
				if (ImGui::BeginMenu("View")) {
					if (ImGui::MenuItem("Edit Mode")) {
						// TODO
					}
					if (ImGui::MenuItem("Painting Mode")) {
						// TODO
					}
					if (ImGui::MenuItem("Spectator Mode")) {
						// TODO
					}
					ImGui::EndMenu(); // Close "Mode" menu
				}

				// "Help" Menu
				if (ImGui::BeginMenu("Help")) {
					if (ImGui::MenuItem("Controls")) {
						// TODO
					}
					if (ImGui::MenuItem("Support")) {
						// TODO
					}
					if (ImGui::MenuItem("Community")) {
						//TODO
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Bug Report")) {
						// TODO
					}
					ImGui::EndMenu(); // Close "Help" Menu
				}

				ImGui::EndMainMenuBar(); // Close main menu bar
			}
		}

	};
}