#include <cmd_move_at.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <sstream>

CmdType MoveCmd::get_type() const
{
    return CmdType::MoveAtXYZ;
}

std::string MoveCmd::get_name() const
{
    return "Move At XYZ";
}

size_t MoveCmd::get_pcounter() const
{
    return my_pcounter;
}

void MoveCmd::set_pcounter(size_t val)
{
    my_pcounter = val;
}

void MoveCmd::on_execute(ICMM *cmm)
{
    // FIXME: threading
    cmm->cmd_move_at(position);
}

void MoveCmd::on_draw_imgui()
{
    float temp[3] = {};

    temp[0] = position.x();
    temp[1] = position.y();
    temp[2] = position.z();
    if(ImGui::InputFloat3("Position", temp)) {
        position = Eigen::Vector3d{temp[0], temp[1], temp[2]};
    }
}

bool MoveCmd::validate()
{
    return true;
}

void MoveCmd::on_load(std::ifstream &file)
{
    size_t offset;
    std::string line {};

    while(std::getline(file, line)) {
        if((offset = line.find("pcounter")) != std::string::npos) {
            my_pcounter = static_cast<size_t>(strtoull(line.substr(offset + 8).c_str(), nullptr, 10));
            continue;
        }

        if((offset = line.find("x")) != std::string::npos) {
            position.x() = atof(line.substr(offset + 1).c_str());
            continue;
        }

        if((offset = line.find("y")) != std::string::npos) {
            position.y() = atof(line.substr(offset + 1).c_str());
            continue;
        }

        if((offset = line.find("z")) != std::string::npos) {
            position.z() = atof(line.substr(offset + 1).c_str());
            continue;
        }

        if(line.find("end") != std::string::npos) {
            break;
        }
    }
}

void MoveCmd::on_save(std::ofstream &file) const
{
    file << "move" << std::endl;
    file << "pcounter" << my_pcounter << std::endl;
    file << "x" << position.x() << std::endl;
    file << "y" << position.y() << std::endl;
    file << "z" << position.z() << std::endl;
    file << "end" << std::endl;
}
