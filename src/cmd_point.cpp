#include <cmd_point.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <sstream>

void PointCmd::set_name(const std::string &name)
{
    this->name = name;
}

const Eigen::Vector3d &PointCmd::get_in_normal() const
{
    return in_normal;
}

const Eigen::Vector3d &PointCmd::get_in_position() const
{
    return in_position;
}

const Eigen::Vector3d &PointCmd::get_real_position() const
{
    return real_position;
}

CmdType PointCmd::get_type() const
{
    return CmdType::MeasurePoint;
}

std::string PointCmd::get_name() const
{
    if(name.empty())
        return "Unnamed PointCmd";
    return name + std::string{" (PointCmd)"};
}

void PointCmd::on_execute(ICMM *cmm)
{
    cmm->cmd_point(in_position, in_normal, real_position);
}

void PointCmd::on_draw_imgui()
{
    float temp[3];
    std::string temp_s = name;

    if(ImGui::InputText("Name", &temp_s)) {
        // FIXME: force validation
        set_name(temp_s);
    }

    temp[0] = in_position.x();
    temp[1] = in_position.y();
    temp[2] = in_position.z();
    if(ImGui::InputFloat3("Position", temp)) {
        in_position = Eigen::Vector3d{temp[0], temp[1], temp[2]};
        real_position = Eigen::Vector3d{};
    }

    temp[0] = in_normal.x();
    temp[1] = in_normal.y();
    temp[2] = in_normal.z();
    if(ImGui::InputFloat3("Normal", temp)) {
        in_normal = Eigen::Vector3d{temp[0], temp[1], temp[2]};
        real_position = Eigen::Vector3d{};
    }

    ImGui::Text("REAL: %.3f %.3f %.3f", real_position.x(), real_position.y(), real_position.z());
}

bool PointCmd::validate()
{
    return true;
}
