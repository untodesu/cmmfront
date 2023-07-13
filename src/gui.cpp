#include <filesystem>
#include <algorithm>
#include <cmd_circle.hpp>
#include <cmd_comment.hpp>
#include <cmd_move_at.hpp>
#include <cmd_point.hpp>
#include <cmd_plane.hpp>
#include <cmd_report.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <gui.hpp>
#include <imgui.h>
#include <iostream>
#include <stdio.h> // snprintf
#include <sstream>
#include <ios>
#include <mutex>
#include <portable-file-dialogs.h>

static std::mutex gui_mutex = {};
static std::filesystem::path last_path = {};

static bool read_file(const std::filesystem::path &path)
{
    std::string line;
    std::ifstream file{path};

    if(!file.good() || !file.is_open()) {
        // Big trouble now
        return false;
    }

    globals::commands.clear();
    globals::selected_command = nullptr;
    globals::current = globals::commands.end();

    while(std::getline(file, line)) {
        if(line.find("comment") != std::string::npos) {
            CommentCmd *comment = new CommentCmd{};
            comment->on_load(file);
            globals::commands.push_back(comment);
            continue;
        }

        if(line.find("move") != std::string::npos) {
            MoveCmd *move = new MoveCmd{};
            move->on_load(file);
            globals::commands.push_back(move);
            continue;
        }

        if(line.find("point") != std::string::npos) {
            PointCmd *point = new PointCmd{};
            point->on_load(file);
            globals::commands.push_back(point);
            continue;
        }

        if(line.find("plane") != std::string::npos) {
            PlaneCmd *plane = new PlaneCmd{};
            plane->on_load(file);
            globals::commands.push_back(plane);
            continue;
        }

        if(line.find("circle") != std::string::npos) {
            CircleCmd *circle = new CircleCmd{};
            circle->on_load(file);
            globals::commands.push_back(circle);
            continue;
        }

        if(line.find("report") != std::string::npos) {
            ReportCmd *report = new ReportCmd{};
            report->on_load(file);
            globals::commands.push_back(report);
            continue;
        }
    }

    last_path = path;

    return true;
}

static bool write_file(const std::filesystem::path &path)
{
    std::ofstream file{path};

    if(!file.good() || !file.is_open()) {
        // Big trouble now
        return false;
    }

    for(const auto it : globals::commands)
        it->on_save(file);
    file.close();

    last_path = path;

    return true;
}

static bool is_shortcut(ImGuiKey key, ImGuiKeyChord mods)
{
    if(ImGui::GetIO().WantTextInput)
        return false;
    if(ImGui::GetIO().KeyMods != mods)
        return false;
    if(!ImGui::IsKeyPressed(key, false))
        return false;
    return true;
}

static void file_new()
{
    globals::commands.clear();
    globals::selected_command = nullptr;
    globals::current = globals::commands.end();
    last_path.clear();
}

static void file_open()
{
    std::filesystem::path cwd = std::filesystem::current_path();
    if(!last_path.empty())
        cwd = last_path.parent_path();
    pfd::open_file diag{"Open", cwd.string(), {"CMM/Front files", "*.cmm"}, pfd::opt::none};
    const std::vector<std::string> result = diag.result();
    if(!result.empty()) {
        read_file(result[0]);
    }
}

static void file_save()
{
    if(last_path.empty()) {
        pfd::save_file diag{"Save as", std::filesystem::current_path().string(), {"CMM/Front files", "*.cmm"}, pfd::opt::none};
        const std::string result = diag.result();
        if(!result.empty()) {
            write_file(result);
        }
    }
    else {
        write_file(last_path);
    }
}

static void file_save_as()
{
    std::filesystem::path cwd = std::filesystem::current_path();
    if(!last_path.empty())
        cwd = last_path.parent_path();
    pfd::save_file diag{"Save as", cwd.string(), {"CMM files", "*.cmm"}, pfd::opt::none};
    const std::string result = diag.result();
    if(!result.empty()) {
        write_file(result);
    }
}

static void add_command(ICmd *cmd)
{
    globals::commands.push_back(cmd);
    globals::selected_command = cmd;
    cmm_wrap::abort();
}

static void cmd_moveup(bool allow_changes, bool &selection_active)
{
    if(allow_changes && !globals::commands.empty()) {
        for(auto it = globals::commands.begin() + 1; it != globals::commands.end(); ++it) {
            if(*it == globals::selected_command) {
                std::swap(*it, *(it - 1));
                selection_active = true;
                cmm_wrap::abort();
                break;
            }
        }
    }
}

static void cmd_movedown(bool allow_changes, bool &selection_active)
{
    if(allow_changes && !globals::commands.empty()) {
        for(auto it = globals::commands.begin(); it != globals::commands.end() - 1; ++it) {
            if(*it == globals::selected_command) {
                std::swap(*it, *(it + 1));
                selection_active = true;
                cmm_wrap::abort();
                break;
            }
        }
    }
}

static void cmd_remove(bool allow_changes, bool &selection_active)
{
    if(allow_changes && !globals::commands.empty() && globals::selected_command) {
        delete globals::selected_command;
        globals::commands.erase(std::remove(globals::commands.begin(), globals::commands.end(), globals::selected_command), globals::commands.end());
        globals::selected_command = nullptr;
        cmm_wrap::abort();
    }
}

static void cmd_run()
{
    if(globals::current == globals::commands.end())
        globals::current = globals::commands.begin();
    globals::is_running_program = true;

    for(auto it : globals::commands) {
        if(!it->validate()) {
            globals::popups.push(Popup {
                .title = "Validation",
                .content = std::string{"Command `"} + std::string{it->get_name()} + std::string{ "' failed validation check!"},
                .abortable = false,
            });
            globals::selected_command = it;
            globals::current = globals::commands.end();
            globals::is_running_program = false;
            break;
        }
    }
}

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

    if(is_shortcut(ImGuiKey_N, ImGuiKey_ModCtrl))
        file_new();
    if(is_shortcut(ImGuiKey_O, ImGuiKey_ModCtrl))
        file_open();
    if(is_shortcut(ImGuiKey_S, ImGuiKey_ModCtrl))
        file_save();
    if(is_shortcut(ImGuiKey_S, ImGuiKey_ModCtrl | ImGuiKey_ModShift))
        file_save_as();

    if(is_shortcut(ImGuiKey_Q, ImGuiKey_ModShift))
        add_command(new PointCmd{});
    if(is_shortcut(ImGuiKey_W, ImGuiKey_ModShift))
        add_command(new PlaneCmd{});
    if(is_shortcut(ImGuiKey_E, ImGuiKey_ModShift))
        add_command(new CircleCmd{});
    
    if(is_shortcut(ImGuiKey_A, ImGuiKey_ModShift))
        add_command(new CommentCmd{});
    if(is_shortcut(ImGuiKey_S, ImGuiKey_ModShift))
        add_command(new MoveCmd{});
    if(is_shortcut(ImGuiKey_D, ImGuiKey_ModShift))
        add_command(new ReportCmd{});

    if(ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        globals::selected_command = nullptr;
    if(is_shortcut(ImGuiKey_UpArrow, ImGuiKey_ModShift))
        cmd_moveup(allow_changes, selection_active);
    if(is_shortcut(ImGuiKey_DownArrow, ImGuiKey_ModShift))
        cmd_movedown(allow_changes, selection_active);
    if(is_shortcut(ImGuiKey_Delete, 0))
        cmd_remove(allow_changes, selection_active);

    if(is_shortcut(ImGuiKey_F5, 0))
        cmd_run();
    if(is_shortcut(ImGuiKey_F6, 0))
        globals::is_running_program = false;
    if(is_shortcut(ImGuiKey_F5, ImGuiKey_ModShift))
        cmm_wrap::abort();

    if(!globals::popups.empty()) {
        globals::is_running_program = false;
        ImGui::OpenPopup(globals::popups.back().title.c_str());
    }

    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("New", "Ctrl+N"))
                file_new();
            if(ImGui::MenuItem("Open", "Ctrl+O"))
                file_open();
            ImGui::Separator();

            if(ImGui::MenuItem("Save", "Ctrl+S"))
                file_save();
            if(ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
                file_save_as();
            ImGui::Separator();

            if(ImGui::MenuItem("Exit"))
                glfwSetWindowShouldClose(globals::window, true);
            ImGui::EndMenu();
        }

        if(globals::machine->get_cap(CmmCap::DebugCMM)) {
            if(ImGui::BeginMenu("Debug")) {
                double temp;
                int temp_int;

                temp = globals::machine->get_opt_double(CmmOpt::DebugRandomness);
                if(ImGui::InputDouble("Randomness", &temp)) {
                    globals::machine->set_opt_double(CmmOpt::DebugRandomness, temp);
                }

                temp = globals::machine->get_opt_double(CmmOpt::DebugTimeDelay);
                if(ImGui::InputDouble("Time Delay", &temp)) {
                    globals::machine->set_opt_double(CmmOpt::DebugTimeDelay, temp);
                }

                temp_int = globals::machine->get_opt_int(CmmOpt::DebugResult);
                if(ImGui::Combo("Result", &temp_int, "Ok\0Aborted\0HardwareFail\0")) {
                    globals::machine->set_opt_int(CmmOpt::DebugResult, temp_int);
                }

                ImGui::EndMenu();
            }
        }

        if(ImGui::BeginMenu("Edit")) {
            if(ImGui::BeginMenu("Add command")) {

                if(ImGui::MenuItem("Measure Point", "Shift+Q"))
                    add_command(new PointCmd{});
                if(ImGui::MenuItem("Measure Plane", "Shift+W"))
                    add_command(new PlaneCmd{});
                if(ImGui::MenuItem("Measure Circle", "Shift+E"))
                    add_command(new CircleCmd{});
                ImGui::Separator();

                if(ImGui::MenuItem("Comment", "Shift+A"))
                    add_command(new CommentCmd{});
                if(ImGui::MenuItem("Move at XYZ", "Shift+S"))
                    add_command(new MoveCmd{});
                if(ImGui::MenuItem("Report", "Shift+D"))
                    add_command(new ReportCmd{});
                ImGui::EndMenu();
            }

            ImGui::Separator();

            if(ImGui::MenuItem("Move Up", "Shift+Up"))
                cmd_moveup(allow_changes, selection_active);
            if(ImGui::MenuItem("Move Down", "Shift+Down"))
                cmd_movedown(allow_changes, selection_active);
            if(ImGui::MenuItem("Remove", "Delete"))
                cmd_remove(allow_changes, selection_active);

            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Run")) {
            if(ImGui::MenuItem("Start", "F5"))
                cmd_run();
            if(ImGui::MenuItem("Pause", "F6"))
                globals::is_running_program = false;
            if(ImGui::MenuItem("Abort", "Shift+F5"))
                cmm_wrap::abort();
            ImGui::EndMenu();
        }

        ImGui::Separator();

        const bool is_running = globals::is_running_program;
        const bool is_busy = globals::machine->is_busy();
        const char *string_status = "[IDLE // STOP]";
        if(is_busy && is_running)
            string_status = "[BUSY // EXEC]";
        else if(is_busy)
            string_status = "[BUSY // STOP]";
        else if(is_running)
            string_status = "[IDLE // EXEC]";
        ImGui::MenuItem(globals::machine->get_ident(), nullptr, nullptr, false);
        ImGui::MenuItem(string_status, nullptr, nullptr, false);

        ImGui::Separator();

        if(!last_path.empty()) {
            ImGui::MenuItem(last_path.string().c_str(), nullptr, nullptr, false);
            ImGui::Separator();
        }

        ImGui::EndMainMenuBar();
    }

    if(ImGui::Begin("Commands"), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize) {
        char stager[128] = {0};
        size_t command_index = 0;

        ImGui::SetWindowPos(ImVec2{16.0f, 32.0f}, ImGuiCond_Always);
        ImGui::SetWindowSize(ImVec2{static_cast<float>(width) - 32.0f, static_cast<float>(height) - 48.0f}, ImGuiCond_Always);

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
    } ImGui::End();

    if(!globals::popups.empty()) {
        gui_mutex.lock();

        const Popup &info = globals::popups.back();

        if(ImGui::BeginPopupModal(info.title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextUnformatted(info.content.c_str());
            ImGui::Separator();

            if(info.abortable) {
                if(ImGui::Button("Abort")) {
                    ImGui::CloseCurrentPopup();
                    globals::popups.pop();
                    cmm_wrap::abort();
                }


                ImGui::SameLine();
            }

            if(ImGui::Button(info.abortable ? "Continue" : "OK")) {
                ImGui::CloseCurrentPopup();
                globals::popups.pop();
                globals::is_running_program = true;
            }

            ImGui::EndPopup();
        }

        gui_mutex.unlock();
    }

    if(!selection_active) {
        globals::selected_command = nullptr;
    }
}
