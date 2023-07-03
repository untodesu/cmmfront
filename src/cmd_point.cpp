#include <cmd_point.hpp>
#include <cmd_plane.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <sstream>
#include <math_plane.hpp>

void PointCmd::set_name(const std::string &name)
{
    this->name = name;
}

const Eigen::Vector3d &PointCmd::get_calc_normal() const
{
    return calc_normal;
}

const Eigen::Vector3d &PointCmd::get_calc_position() const
{
    return calc_position;
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

size_t PointCmd::get_pcounter() const
{
    return my_pcounter;
}

void PointCmd::set_pcounter(size_t val)
{
    my_pcounter = val;
}

void PointCmd::on_execute(ICMM *cmm)
{
    switch(point_type) {
        case PointType::Generic:
            cmm->cmd_point(calc_position, calc_normal, real_position);
            break;
        case PointType::PlaneProj:
            calc_position = math::plane_proj(target_plane->get_calc_position(), target_plane->get_calc_normal(), calc_proj_target);
            real_position = math::plane_proj(target_plane->get_real_position(), target_plane->get_real_normal(), calc_proj_target);
            break;
    }
}

void PointCmd::on_draw_imgui()
{
    float temp[3];
    std::string temp_s = name;

    if(ImGui::InputText("Name", &temp_s)) {
        for(auto it : globals::commands) {
            if(it != this && reinterpret_cast<PointCmd *>(it)->name == temp_s) {
                // Ensure things being unique
                temp_s += "1";
            }
        }

        set_name(temp_s);
    }

    int index = static_cast<int>(point_type);
    if(ImGui::Combo("Type", &index, "Generic\0Plane Projection\0Circle Center\0")) {
        // A new point type
        point_type = static_cast<PointType>(index);
    }

    if(point_type == PointType::Generic) {
        temp[0] = calc_position.x();
        temp[1] = calc_position.y();
        temp[2] = calc_position.z();
        if(ImGui::InputFloat3("Position", temp)) {
            calc_position = Eigen::Vector3d{temp[0], temp[1], temp[2]};
            real_position = Eigen::Vector3d{};
        }

        temp[0] = calc_normal.x();
        temp[1] = calc_normal.y();
        temp[2] = calc_normal.z();
        if(ImGui::InputFloat3("Normal", temp)) {
            calc_normal = Eigen::Vector3d{temp[0], temp[1], temp[2]};
            real_position = Eigen::Vector3d{};
        }
    }

    if(point_type == PointType::PlaneProj) {
        temp[0] = calc_proj_target.x();
        temp[1] = calc_proj_target.y();
        temp[2] = calc_proj_target.z();
        if(ImGui::InputFloat3("Target", temp)) {
            calc_proj_target = Eigen::Vector3d{temp[0], temp[1], temp[2]};
            if(target_plane)
                calc_position = math::plane_proj(target_plane->get_calc_position(), target_plane->get_calc_normal(), calc_proj_target);
            else
                calc_position = Eigen::Vector3d{};
            real_position = Eigen::Vector3d{};
        }
    }

    if(point_type == PointType::PlaneProj) {
        if(ImGui::ListBoxHeader("Plane")) {
            bool flag = false;
            for(auto it : globals::commands) {
                if(it->get_pcounter() < my_pcounter && it->get_type() == CmdType::MeasurePlane) {
                    PlaneCmd *pcmd = reinterpret_cast<PlaneCmd *>(it);
                    bool selected = (target_plane == pcmd);
                    flag = true;
                    if(!ImGui::Selectable(it->get_name().c_str(), &selected))
                        continue;
                    target_plane = pcmd;
                    calc_position = math::plane_proj(target_plane->get_calc_position(), target_plane->get_calc_normal(), calc_proj_target);
                    real_position = Eigen::Vector3d{};
                }
            }

            if(!flag) {
                ImGui::Selectable("[No planes?]");
            }

            ImGui::ListBoxFooter();
        }
    }

    ImGui::Text("Real Position: %.3f %.3f %.3f", real_position.x(), real_position.y(), real_position.z());
}

bool PointCmd::validate()
{
    if(point_type == PointType::PlaneProj) {
        if(target_plane) {
            for(auto it : globals::commands) {
                if(it != target_plane)
                    continue;
                return true;
            }

            // Invalid pointer
            return false;
        }

        // No planes?
        return false;
    }

    return true;
}
