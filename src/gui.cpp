#include <algorithm>
#include <cmd_comment.hpp>
#include <cmd_move_at.hpp>
#include <cmd_point.hpp>
#include <cmd_plane.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <gui.hpp>
#include <imgui.h>
#include <iostream>
#include <stdio.h> // snprintf

void gui::draw(int width, int height)
{
    // True whenever there is a popup, or we are running
    // the program. Changes on the fly is a cool thing but
    // pretty much an unnececssary one in this case.
    bool allow_changes = !globals::is_running_program;

    // True when this frame has done something
    // with the selected command so we don't
    // automatically de-select it at the end.
    bool selection_active = false;

    if(!globals::popup_class.empty()) {
        globals::is_running_program = false;
        ImGui::OpenPopup(globals::popup_class.c_str());
    }

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
            if(globals::current == globals::commands.end())
                globals::current = globals::commands.begin();
            globals::is_running_program = true;

            for(auto it : globals::commands) {
                if(!it->validate()) {
                    globals::popup_class = "CommandList";
                    globals::popup_text = "Command " + it->get_name() + " failed validation check!";
                    globals::is_running_program = false;
                    break;
                }
            }
        }

        ImGui::SameLine();
        if(ImGui::Button("Pause")) {
            // Pause the execution, normal flow
            // can be continued by pressing Run again
            // or user can run commands step-by-step
            globals::is_running_program = false;
        }

        ImGui::SameLine();
        if(ImGui::Button("Step") && !globals::is_running_program) {
            if(globals::current == globals::commands.end())
                globals::current = globals::commands.begin();
            cmm_wrap::step();
        }

        ImGui::SameLine();
        if(ImGui::Button("Abort")) {
            // Stops the program execution at all,
            // resetting the internal program counter
            cmm_wrap::abort();
        }

        ImGui::SeparatorText("Add");

        if(ImGui::Button("Comment") && allow_changes) {
            globals::commands.push_back(new CommentCmd{});
            cmm_wrap::abort();
        }

        ImGui::SameLine();
        if(ImGui::Button("MoveAtXYZ") && allow_changes) {
            globals::commands.push_back(new MoveCmd{});
            cmm_wrap::abort();
        }

        ImGui::SameLine();
        if(ImGui::Button("Point") && allow_changes) {
            globals::commands.push_back(new PointCmd{});
            cmm_wrap::abort();
        }

        if(ImGui::Button("Plane") && allow_changes) {
            globals::commands.push_back(new PlaneCmd{});
            cmm_wrap::abort();
        }

        ImGui::SameLine();
        if(ImGui::Button("Circle") && allow_changes) {
            std::cerr << "Unimplemented!" << std::endl;
        }

        if(ImGui::Button("Report") && allow_changes) {
            std::cerr << "Unimplemented!" << std::endl;
        }

        ImGui::SeparatorText("Manage");

        if(ImGui::Button("Move Up") && allow_changes && !globals::commands.empty()) {
            for(auto it = globals::commands.begin() + 1; it != globals::commands.end(); ++it) {
                if(*it == globals::selected_command) {
                    std::swap(*it, *(it - 1));
                    selection_active = true;
                    cmm_wrap::abort();
                    break;
                }
            }
        }

        ImGui::SameLine();
        if(ImGui::Button("Move Down") && allow_changes && !globals::commands.empty()) {
            for(auto it = globals::commands.begin(); it != globals::commands.end() - 1; ++it) {
                if(*it == globals::selected_command) {
                    std::swap(*it, *(it + 1));
                    selection_active = true;
                    cmm_wrap::abort();
                    break;
                }
            }
        }

        if(ImGui::Button("Remove") && allow_changes && !globals::commands.empty()) {
            if(globals::selected_command) {
                delete globals::selected_command;
                globals::commands.erase(std::remove(globals::commands.begin(), globals::commands.end(), globals::selected_command), globals::commands.end());
                globals::selected_command = nullptr;
                cmm_wrap::abort();
            }
        }

        ImGui::SeparatorText("Commands");
        if(ImGui::BeginChild("CommandList")) {
            char stager[128] = {0};
            size_t command_index = 0;

            for(ICmd *it : globals::commands) {
                it->set_pcounter(command_index++);

                // ImGui breaks if the collapsing headers
                // have the same name (ie CommentCommand), so we
                // have to introduce some unique-ness to them by
                // prefixing them with a hexadecimal index.
                // I don't know a better way than to snprintf it.
                snprintf(stager, sizeof(stager), "[%04zX] %s", it->get_pcounter(), it->get_name().c_str());

                // Ensure the selected command is visible
                // while de-selected commands are not
                ImGui::SetNextItemOpen(globals::selected_command == it);

                if(ImGui::CollapsingHeader(stager)) {
                    if(allow_changes)
                        it->on_draw_imgui();
                    globals::selected_command = it;
                    selection_active = true;
                }
            }
        } ImGui::EndChild();
    } ImGui::End();

    if(!globals::popup_text.empty() && ImGui::BeginPopupModal(globals::popup_class.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextUnformatted(globals::popup_text.c_str());
        if(ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
            globals::popup_class.clear();
            globals::popup_text.clear();
            globals::is_running_program = true;
        }

        ImGui::EndPopup();
    }

    if(!selection_active) {
        globals::selected_command = nullptr;
    }
}
