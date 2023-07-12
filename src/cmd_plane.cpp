#include <cmd_plane.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>

void PlaneCmd::set_name(const std::string &name)
{
    this->name = name;
}

const Plane &PlaneCmd::get_calc_plane() const
{
    return calc_plane;
}

const Plane &PlaneCmd::get_real_plane() const
{
    return real_plane;
}

CmdType PlaneCmd::get_type() const
{
    return CmdType::MeasurePlane;
}

std::string PlaneCmd::get_name() const
{
    if(name.empty())
        return std::string{"Unnamed PlaneCmd"};
    return name + std::string{" (PlaneCmd)"};
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
    calc_plane.set_guess(points);
    real_plane.set_actual(points);
}

void PlaneCmd::on_draw_imgui()
{
    char stager[128] = {0};
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

    const Eigen::Vector3d &calc_point = calc_plane.get_point();
    const Eigen::Vector3d &calc_normal = calc_plane.get_normal();
    const Eigen::Vector3d &real_point = real_plane.get_point();
    const Eigen::Vector3d &real_normal = real_plane.get_normal();
    ImGui::Text("Guessed point: %.3f %.3f %.3f", calc_point.x(), calc_point.y(), calc_point.z());
    ImGui::Text("Guessed normal: %.3f %.3f %.3f", calc_normal.x(), calc_normal.y(), calc_normal.z());
    ImGui::Text("Actual point: %.3f %.3f %.3f", real_point.x(), real_point.y(), real_point.z());
    ImGui::Text("Actual normal: %.3f %.3f %.3f", real_normal.x(), real_normal.y(), real_normal.z());

    if(ImGui::ListBoxHeader("Unselected points")) {
        for(auto it : globals::commands) {
            if(it->get_type() == CmdType::MeasurePoint && it->get_pcounter() < my_pcounter) {
                PointCmd *pit = reinterpret_cast<PointCmd *>(it);
                if(points.count(pit) == 0) {
                    snprintf(stager, sizeof(stager), "%s [%04zX]", pit->get_name().c_str(), pit->get_pcounter());
                    if(!ImGui::Selectable(stager))
                        continue;
                    points.insert(pit);
                    calc_plane.set_guess(points);
                }
            }
        }

        ImGui::ListBoxFooter();
    }

    if(ImGui::ListBoxHeader("Selected points")) {
        for(auto it = points.begin(); it != points.end(); ++it) {
            if((*it)->get_pcounter() > my_pcounter) {
                it = points.erase(it);
                calc_plane.set_guess(points);
                if(it != points.end())
                    continue;
                break;
            }

            snprintf(stager, sizeof(stager), "%s [%04zX]", (*it)->get_name().c_str(), (*it)->get_pcounter());
            if(!ImGui::Selectable(stager))
                continue;
            it = points.erase(it);
            calc_plane.set_guess(points);

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

void PlaneCmd::json_import(const rapidjson::Document &json, size_t pcounter)
{

}

void PlaneCmd::json_export(rapidjson::Document &json, size_t pcounter) const
{

}
