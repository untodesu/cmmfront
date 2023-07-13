#include <cmd_comment.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <sstream>

CmdType CommentCmd::get_type() const
{
    return CmdType::Comment;
}

std::string CommentCmd::get_name() const
{
    if(!comment_text.empty())
        return comment_text;
    return "Empty Comment";
}

size_t CommentCmd::get_pcounter() const
{
    return my_pcounter;
}

void CommentCmd::set_pcounter(size_t val)
{
    my_pcounter = val;
}

bool CommentCmd::on_execute(ICMM *cmm)
{
    if(active) {
        if(!comment_text.empty()) {
            globals::is_running_program = false;
            globals::popups.push(Popup {
                .title = "Comment",
                .content = comment_text,
                .abortable = true,
            });
        }
        else {
            globals::is_running_program = false;
            globals::popups.push(Popup {
                .title = "Comment",
                .content = "[Empty Comment]",
                .abortable = true,
            });
        }
    }

    return true;
}

void CommentCmd::on_draw_imgui()
{
    ImGui::Checkbox("Active", &active);
    ImGui::InputTextMultiline("Text", &comment_text);
}

bool CommentCmd::validate()
{
    return true;
}

void CommentCmd::on_load(std::ifstream &file)
{
    size_t offset;
    std::string line {};

    while(std::getline(file, line)) {
        if((offset = line.find("pcounter")) != std::string::npos) {
            my_pcounter = static_cast<size_t>(strtoull(line.substr(offset + 8).c_str(), nullptr, 10));
            continue;
        }

        if((offset = line.find("inactive")) != std::string::npos) {
            active = false;
            continue;
        }

        if((offset = line.find("text")) != std::string::npos) {
            comment_text += line.substr(offset + 4);
            comment_text += "\n";
            continue;
        }

        if(line.find("end") != std::string::npos) {
            break;
        }
    }
}

void CommentCmd::on_save(std::ofstream &file) const
{
    std::string line {};
    std::istringstream ss {comment_text};
    file << "comment" << std::endl;
    file << "pcounter" << my_pcounter << std::endl;
    while(std::getline(ss, line))
        file << "text" << line << std::endl;
    if(!active)
    file << "inactive" << std::endl;
    file << "end" << std::endl;
}
