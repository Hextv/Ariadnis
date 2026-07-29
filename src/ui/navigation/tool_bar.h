#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

namespace UI {
    class ToolBar {
    public:
        void render() {
            float topBarHeight = ImGui::GetFrameHeight();

            // Get window display size
            ImGuiIO& io = ImGui::GetIO();
            float panelWidth = 60.0f; // Width for tool icons/buttons
            float panelHeight = io.DisplaySize.y - topBarHeight;

            // Pin position beneath the top bar and set fixed width/height
            ImGui::SetNextWindowPos(ImVec2(0.0f, topBarHeight), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight), ImGuiCond_Always);

            // Window flags to lock panel in place
            ImGuiWindowFlags windowFlags =
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6.0f, 6.0f));

            if (ImGui::Begin("ToolBarPanel", nullptr, windowFlags)) {

                // TODO: TOOL BUTTONS

                // Raise / Lower Brush
                if (ImGui::Button("R", ImVec2(48, 48))) {
                    // TODo
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Raise / Lower Brush");
                }

                ImGui::Spacing();

                // Smooth Tool
                if (ImGui::Button("S", ImVec2(48, 48))) {
                    // Action: Select Smooth Tool
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Smooth Terrain Tool");
                }

                ImGui::End();
            }

            ImGui::PopStyleVar();
        }
    };
}