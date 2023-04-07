#include <algorithm>
#include <cmd_comment.hpp>
#include <globals.hpp>
#include <gui.hpp>
#include <imgui.h>
#include <iostream>
#include <stdio.h> // snprintf

void gui::draw(int width, int height)
{
    // True when this frame has done something
    // with the selected command so we don't
    // automatically de-select it at the end.
    bool selection_active = false;

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
        if(ImGui::Button("Run")) {
            std::cerr << "Unimplemented!" << std::endl;
        }

        ImGui::SameLine();
        if(ImGui::Button("Pause")) {
            std::cerr << "Unimplemented!" << std::endl;
        }

        ImGui::SameLine();
        if(ImGui::Button("Step")) {
            std::cerr << "Unimplemented!" << std::endl;
        }

        ImGui::SameLine();
        if(ImGui::Button("Abort")) {
            std::cerr << "Unimplemented!" << std::endl;
        }

        // When the flag is set, program execution stops
        // at the first command/instruction that is considered selected
        ImGui::Checkbox("Stop at selection", &globals::run_until_selection);

        ImGui::SeparatorText("Add");

        if(ImGui::Button("Comment")) { globals::commands.push_back(new CommentCmd{}); }

        ImGui::SameLine();
        if(ImGui::Button("MoveAtXYZ")) { std::cerr << "Unimplemented!" << std::endl; }

        ImGui::SameLine();
        if(ImGui::Button("Point")) { std::cerr << "Unimplemented!" << std::endl; }

        if(ImGui::Button("Plane")) { std::cerr << "Unimplemented!" << std::endl; }

        ImGui::SameLine();
        if(ImGui::Button("Circle")) { std::cerr << "Unimplemented!" << std::endl; }

        if(ImGui::Button("Report")) { std::cerr << "Unimplemented!" << std::endl; }

        ImGui::SeparatorText("Manage");

        if(ImGui::Button("Move Up")) {
            for(auto it = globals::commands.begin() + 1; it != globals::commands.end(); ++it) {
                if(*it == globals::selected_command) {
                    std::swap(*it, *(it - 1));
                    selection_active = true;
                    break;
                }
            }
        }

        ImGui::SameLine();
        if(ImGui::Button("Move Down")) {
            for(auto it = globals::commands.begin(); it != globals::commands.end() - 1; ++it) {
                if(*it == globals::selected_command) {
                    std::swap(*it, *(it + 1));
                    selection_active = true;
                    break;
                }
            }
        }

        if(ImGui::Button("Remove")) {
            if(globals::selected_command) {
                globals::commands.erase(std::remove(globals::commands.begin(), globals::commands.end(), globals::selected_command), globals::commands.end());
                globals::selected_command = nullptr;
            }
        }

        ImGui::SeparatorText("Commands");
        if(ImGui::BeginChild("CommandList")) {
            char stager[128] = {0};
            size_t command_index = 0;

            for(ICmd *it : globals::commands) {
                // ImGui breaks if the collapsing headers
                // have the same name (ie CommentCommand), so we
                // have to introduce some unique-ness to them by
                // prefixing them with a hexadecimal index.
                // I don't know a better way than to snprintf it.
                snprintf(stager, sizeof(stager), "[%04zX] %s", command_index++, it->get_name());

                // Ensure the selected command is visible
                // while de-selected commands are not
                ImGui::SetNextItemOpen(globals::selected_command == it);

                if(ImGui::CollapsingHeader(stager)) {
                    globals::selected_command = it;
                    selection_active = true;
                    it->on_draw_imgui();
                }
            }
        } ImGui::EndChild();
    } ImGui::End();

    if(!selection_active) {
        globals::selected_command = nullptr;
    }
}
