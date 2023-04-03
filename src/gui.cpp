#include <globals.hpp>
#include <gui.hpp>
#include <imgui.h>

// UNDONE: move this to globals
static bool run_until_selection = false;

// Proportional UI scaling
static ImVec2 remap_vec2(const ImVec2 &vec, int width, int height)
{
    ImVec2 result = {};
    result.x = (vec.x / 640.0f) * static_cast<float>(width);
    result.y = (vec.y / 480.0f) * static_cast<float>(height);
    return result;
}

void gui::draw(int width, int height)
{
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            ImGui::MenuItem("New", "Ctrl+N");
            ImGui::MenuItem("Open", "Ctrl+O");
            ImGui::MenuItem("Save", "Ctrl+S");
            ImGui::MenuItem("Save As", "Ctrl+Shift+S");
            ImGui::Separator();

            if(ImGui::MenuItem("Exit"))
                glfwSetWindowShouldClose(globals::window, true);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if(ImGui::Begin("Commands"), nullptr, ImGuiWindowFlags_NoSavedSettings) {
        ImGui::Button("Run");
        ImGui::SameLine();
        ImGui::Button("Pause");
        ImGui::SameLine();
        ImGui::Button("Step");
        ImGui::SameLine();
        ImGui::Button("Abort");
        ImGui::Checkbox("Stop at selection", &run_until_selection);
        ImGui::SeparatorText("Add");
        ImGui::Button("Comment");
        ImGui::SameLine();
        ImGui::Button("MoveAtXYZ");
        ImGui::SameLine();
        ImGui::Button("Point");
        ImGui::Button("Plane");
        ImGui::SameLine();
        ImGui::Button("Circle");
        ImGui::Button("Report");
        ImGui::SeparatorText("Manage");
        ImGui::Button("Move Up");
        ImGui::SameLine();
        ImGui::Button("Move Down");
        ImGui::Button("Remove");
        ImGui::SeparatorText("Commands");

        if(ImGui::BeginChild("CommandList")) {
            if(ImGui::CollapsingHeader("[0000] WhateverCommand")) {
                ImGui::Text("Here would be some command options");
                ImGui::Text("But for now there is just this stub text");
                ImGui::Separator();
                ImGui::Text("When unwrapped like that, a command is considered selected");
            }

            // UNDONE: go in a loop and draw commands like that
            // UNDONE: set the header color for a command that is currently
            // being executed (beheaded?!) to green or something else to show
            // that this command is being executed (electric chair?!)
            // UNDONE: a way to select a command so multiple commands
            // can be unwrapped to view whatever the hell is going on in the CMM.
            if(ImGui::CollapsingHeader("[0001] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[0002] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[0003] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[0004] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[0005] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[0006] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[0007] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[0008] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[0009] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[000A] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }
            if(ImGui::CollapsingHeader("[000B] EmptyCommand")) { ImGui::Text("Insert a rickroll here"); }

            if(ImGui::CollapsingHeader("[000C] AnotherCommand")) {
                ImGui::Text("Here would be some command options");
                ImGui::Text("But for now there is just this stub text");
                ImGui::Separator();
            }
        } ImGui::EndChild();
    } ImGui::End();

}
