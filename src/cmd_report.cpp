#include <cmd_circle.hpp>
#include <cmd_plane.hpp>
#include <cmd_point.hpp>
#include <cmd_report.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>

void ReportCmd::set_name(const std::string &name)
{
    this->name = name;
}

CmdType ReportCmd::get_type() const
{
    return CmdType::Report;
}

std::string ReportCmd::get_name() const
{
    if(name.empty())
        return std::string{"Unnamed ReportCmd"};
    return name + std::string{" (ReportCmd)"};
}

size_t ReportCmd::get_pcounter() const
{
    return my_pcounter;
}

void ReportCmd::set_pcounter(size_t val)
{
    my_pcounter = val;
}

void ReportCmd::on_execute(ICMM *cmm)
{
    // Nothing, it does everything in on_draw_imgui
}

void ReportCmd::on_draw_imgui()
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

    if(ImGui::ListBoxHeader("Target")) {
        bool flag = false;
        for(auto it : globals::commands) {
            if(it->get_pcounter() < my_pcounter) {
                switch(it->get_type()) {
                    case CmdType::MeasurePoint:
                    case CmdType::MeasurePlane:
                    case CmdType::MeasureCircle:
                        bool selected = (it == target);
                        snprintf(stager, sizeof(stager), "%s [%04zX]", it->get_name().c_str(), it->get_pcounter());
                        if(ImGui::Selectable(stager, &selected))
                            target = it;
                        break;
                }
            }
        }

        ImGui::ListBoxFooter();
    }

    if(target) {
        CmdType type = target->get_type();

        if(type == CmdType::MeasurePoint) {
            const PointCmd *point = reinterpret_cast<const PointCmd *>(target);
            const Eigen::Vector3d calc_normal = point->get_calc_normal();
            const Eigen::Vector3d calc_point = point->get_calc_point();
            const Eigen::Vector3d real_normal = point->get_real_normal();
            const Eigen::Vector3d real_point = point->get_real_point();
            ImGui::Text("Guessed Normal: %.3f %.3f %.3f", calc_normal.x(), calc_normal.y(), calc_normal.z());
            ImGui::Text("Guessed Point: %.3f %.3f %.3f", calc_point.x(), calc_point.y(), calc_point.z());
            ImGui::Text("Actual Normal: %.3f %.3f %.3f", real_normal.x(), real_normal.y(), real_normal.z());
            ImGui::Text("Actual Point: %.3f %.3f %.3f", real_point.x(), real_point.y(), real_point.z());
            const Eigen::Vector3d ndelta = Eigen::Vector3d{real_normal - calc_normal};
            const Eigen::Vector3d pdelta = Eigen::Vector3d{real_point - calc_point};
            ImGui::Text("Normal delta: %.5f %.5f %.5f", ndelta.x(), ndelta.y(), ndelta.z());
            ImGui::Text("Position delta: %.5f %.5f %.5f (%.5f)", pdelta.x(), pdelta.y(), pdelta.z(), pdelta.norm());
            return;
        }

        if(type == CmdType::MeasurePlane) {
            const PlaneCmd *plane = reinterpret_cast<const PlaneCmd *>(target);
            const Eigen::Vector3d calc_normal = plane->get_calc_plane().get_normal();
            const Eigen::Vector3d calc_point = plane->get_calc_plane().get_point();
            const Eigen::Vector3d real_normal = plane->get_real_plane().get_normal();
            const Eigen::Vector3d real_point = plane->get_real_plane().get_point();
            ImGui::Text("Guessed Normal: %.3f %.3f %.3f", calc_normal.x(), calc_normal.y(), calc_normal.z());
            ImGui::Text("Guessed Point: %.3f %.3f %.3f", calc_point.x(), calc_point.y(), calc_point.z());
            ImGui::Text("Actual Normal: %.3f %.3f %.3f", real_normal.x(), real_normal.y(), real_normal.z());
            ImGui::Text("Actual Point: %.3f %.3f %.3f", real_point.x(), real_point.y(), real_point.z());
            const Eigen::Vector3d normal_delta = Eigen::Vector3d{real_normal - calc_normal};
            const Eigen::Vector3d point_delta = Eigen::Vector3d{real_point - calc_point};
            ImGui::Text("Normal delta: %.5f %.5f %.5f", normal_delta.x(), normal_delta.y(), normal_delta.z());
            ImGui::Text("Point delta: %.5f %.5f %.5f (%.6f)", point_delta.x(), point_delta.y(), point_delta.z(), point_delta.norm());
            return;
        }

        if(type == CmdType::MeasureCircle) {
            const CircleCmd *circle = reinterpret_cast<const CircleCmd *>(target);
            const Eigen::Vector3d &calc_center = circle->get_calc_point();
            const Eigen::Vector3d &real_center = circle->get_real_point();
            const double calc_radius = circle->get_calc_radius();
            const double real_radius = circle->get_real_radius();
            ImGui::Text("Guessed center: %.3f %.3f %.3f", calc_center.x(), calc_center.y(), calc_center.z());
            ImGui::Text("Guessed radius: %.6f", calc_radius);
            ImGui::Text("Actual center: %.3f %.3f %.3f", real_center.x(), real_center.y(), real_center.z());
            ImGui::Text("Actual radius: %.6f", real_radius);
            const Eigen::Vector3d center_delta = Eigen::Vector3d{real_center - calc_center};
            const double radius_delta = abs(real_radius - calc_radius);
            ImGui::Text("Center delta: %.5f %.5f %.5f (%.6f)", center_delta.x(), center_delta.y(), center_delta.z(), center_delta.norm());
            ImGui::Text("Radius delta: %.6f", radius_delta);
            return;
        }
    }
}

bool ReportCmd::validate()
{
    if(target) {
        for(auto it : globals::commands) {
            if(it == target) {
                switch(target->get_type()) {
                    case CmdType::MeasurePoint:
                    case CmdType::MeasurePlane:
                    case CmdType::MeasureCircle:
                        return true; // reportable
                    default:
                        return false; // non-reportable
                }
            }
        }

        // Invalid target
        return false;
    }

    // No target
    return false;
}
