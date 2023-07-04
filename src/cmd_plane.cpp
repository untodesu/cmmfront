#include <cmd_plane.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <math_plane.hpp>

void PlaneCmd::set_name(const std::string &name)
{
    this->name = name;
}

const Eigen::Vector3d &PlaneCmd::get_calc_normal() const
{
    return calc_normal;
}

const Eigen::Vector3d &PlaneCmd::get_calc_point() const
{
    return calc_point;
}

const Eigen::Vector3d &PlaneCmd::get_real_normal() const
{
    return real_normal;
}

const Eigen::Vector3d &PlaneCmd::get_real_point() const
{
    return real_point;
}

CmdType PlaneCmd::get_type() const
{
    return CmdType::MeasurePlane;
}

std::string PlaneCmd::get_name() const
{
    if(name.empty())
        return std::string{"Unnamed PlaneCmd"};
    return name;
}

size_t PlaneCmd::get_pcounter() const
{
    return my_pcounter;
}

void PlaneCmd::set_pcounter(size_t val)
{
    my_pcounter = val;
}

void PlaneCmd::on_execute(ICMM *cmm)
{
    math::plane_real(points, real_point, real_normal);
}

void PlaneCmd::on_draw_imgui()
{
    float temp[3];
    std::string temp_s = name;

    if(ImGui::InputText("Name", &temp_s)) {
        for(auto it : globals::commands) {
            if(it != this && it->get_name() == temp_s) {
                // Ensure things being unique
                temp_s += "1";
            }
        }

        set_name(temp_s);
    }

    ImGui::Text("Guessed point: %.3f %.3f %.3f", calc_point.x(), calc_point.y(), calc_point.z());
    ImGui::Text("Guessed normal: %.3f %.3f %.3f", calc_normal.x(), calc_normal.y(), calc_normal.z());
    ImGui::Text("Actual point: %.3f %.3f %.3f", real_point.x(), real_point.y(), real_point.z());
    ImGui::Text("Actual normal: %.3f %.3f %.3f", real_normal.x(), real_normal.y(), real_normal.z());

    if(ImGui::ListBoxHeader("Unselected points")) {
        for(auto it : globals::commands) {
            if(it->get_type() == CmdType::MeasurePoint && it->get_pcounter() < my_pcounter) {
                PointCmd *pit = reinterpret_cast<PointCmd *>(it);
                if(points.count(pit) == 0) {
                    if(!ImGui::Selectable(pit->get_name().c_str()))
                        continue;
                    points.insert(pit);
                    math::plane_calc(points, calc_point, calc_normal);
                }
            }
        }

        ImGui::ListBoxFooter();
    }

    if(ImGui::ListBoxHeader("Selected points")) {
        for(auto it = points.begin(); it != points.end(); ++it) {
            if((*it)->get_pcounter() > my_pcounter) {
                it = points.erase(it);
                math::plane_calc(points, calc_point, calc_normal);
                if(it != points.end())
                    continue;
                break;
            }

            if(!ImGui::Selectable((*it)->get_name().c_str()))
                continue;
            it = points.erase(it);
            math::plane_calc(points, calc_point, calc_normal);

            // YIKES!
            if(it != points.end())
                continue;
            break;
        }
        ImGui::ListBoxFooter();
    }
}

bool PlaneCmd::validate()
{
    std::unordered_set<const PointCmd *> global_points = {};

    if(points.size() >= 3) {
        for(auto it : globals::commands) {
            if(it->get_type() != CmdType::MeasurePoint)
                continue;
            global_points.insert(reinterpret_cast<const PointCmd *>(it));
        }

        for(auto it : points) {
            if(global_points.count(it) == 0 || it->get_pcounter() > my_pcounter) {
                // We did an oopsie
                return false;
            }
        }

        return true;
    }

    // Not enough points
    return false;
}
