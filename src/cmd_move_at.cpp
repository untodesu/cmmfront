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

void MoveCmd::json_import(const rapidjson::Document &json, size_t pcounter)
{

}

void MoveCmd::json_export(rapidjson::Document &json, size_t pcounter) const
{

}
