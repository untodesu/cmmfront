#include <cmd_circle.hpp>
#include <cmm_wrap.hpp>
#include <globals.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>

void CircleCmd::set_name(const std::string &name)
{
    this->name = name;
}

const Plane &CircleCmd::get_calc_plane() const
{
    return calc_plane;
}

const Plane &CircleCmd::get_real_plane() const
{
    return real_plane;
}

const Eigen::Vector3d &CircleCmd::get_calc_point() const
{
    return calc_center;
}

const Eigen::Vector3d &CircleCmd::get_real_point() const
{
    return real_center;
}

const float CircleCmd::get_calc_radius() const
{
    return calc_radius;
}

const float CircleCmd::get_real_radius() const
{
    return real_radius;
}

CmdType CircleCmd::get_type() const
{
    return CmdType::MeasureCircle;
}

std::string CircleCmd::get_name() const
{
    if(name.empty())
        return std::string{"Unnamed CircleCmd"};
    return name + std::string{" (CircleCmd)"};
}

size_t CircleCmd::get_pcounter() const
{
    return my_pcounter;
}

void CircleCmd::set_pcounter(size_t val)
{
    my_pcounter = val;
}

void CircleCmd::on_execute(ICMM *cmm)
{
    solve_calc();
    solve_real();
}

void CircleCmd::on_draw_imgui()
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

    ImGui::Text("Guessed center: %.3f %.3f %.3f", calc_center.x(), calc_center.y(), calc_center.z());
    ImGui::Text("Guessed radius: %.6f", calc_radius);
    ImGui::Text("Actual center: %.3f %.3f %.3f", real_center.x(), real_center.y(), real_center.z());
    ImGui::Text("Actual radius: %.6f", real_radius);

    if(ImGui::ListBoxHeader("Unselected points")) {
        for(auto it : globals::commands) {
            if(it->get_type() == CmdType::MeasurePoint && it->get_pcounter() < my_pcounter) {
                PointCmd *pit = reinterpret_cast<PointCmd *>(it);
                if(points.count(pit) == 0) {
                    snprintf(stager, sizeof(stager), "%s [%04zX]", pit->get_name().c_str(), pit->get_pcounter());
                    if(!ImGui::Selectable(stager))
                        continue;
                    points.insert(pit);
                    solve_calc();
                    real_center.setZero();
                }
            }
        }

        ImGui::ListBoxFooter();
    }

    if(ImGui::ListBoxHeader("Selected points")) {
        for(auto it = points.begin(); it != points.end(); ++it) {
            if((*it)->get_pcounter() > my_pcounter) {
                it = points.erase(it);
                solve_calc();
                real_center.setZero();
                if(it != points.end())
                    continue;
                break;
            }

            snprintf(stager, sizeof(stager), "%s [%04zX]", (*it)->get_name().c_str(), (*it)->get_pcounter());
            if(!ImGui::Selectable(stager))
                continue;
            it = points.erase(it);
            solve_calc();
            real_center.setZero();

            // YIKES!
            if(it != points.end())
                continue;
            break;
        }
        ImGui::ListBoxFooter();
    }
}

bool CircleCmd::validate()
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

void CircleCmd::solve_calc()
{
    std::vector<Eigen::Vector3d> pts = {};

    // Reset things
    calc_center.setZero();
    calc_radius = double{0.0};

    if(points.size() >= 3) {
        calc_plane.set_guess(points);

        for(auto it : points) {
            // Project every single point onto the plane
            pts.push_back(calc_plane.project2d(it->get_calc_point()));
        }

        const double X12 = pts[0].x() - pts[1].x();
        const double X13 = pts[0].x() - pts[2].x();
        const double Y12 = pts[0].z() - pts[1].z();
        const double Y13 = pts[0].z() - pts[2].z();
        const double Y31 = pts[2].z() - pts[0].z();
        const double Y21 = pts[1].z() - pts[0].z();
        const double X31 = pts[2].x() - pts[0].x();
        const double X21 = pts[1].x() - pts[0].x();

        const double SX13 = pow(pts[0].x(), 2.0) - pow(pts[2].x(), 2.0);
        const double SY13 = pow(pts[0].z(), 2.0) - pow(pts[2].z(), 2.0);
        const double SX21 = pow(pts[1].x(), 2.0) - pow(pts[0].x(), 2.0);
        const double SY21 = pow(pts[1].z(), 2.0) - pow(pts[0].z(), 2.0);

        const double f = ((SX13 * X12) + (SY13 * X12) + (SX21 * X13) + (SY21 * X13)) / (2.0 * ((Y31 * X12) - (Y21 * X13)));
        const double g = ((SX13 * Y12) + (SY13 * Y12) + (SX21 * Y13) + (SY21 * Y13)) / (2.0 * ((X31 * Y12) - (X21 * Y13)));
        const double c = -1.0 * pow(pts[0].x(), 2.0) - pow(pts[0].x(), 2.0) - 2.0 * g * pts[0].x() - 2.0 * f * pts[0].z();

        // Circle's center
        const Eigen::Vector3d center = {-g, 0.0, -f};
        calc_center = calc_plane.unproject2d(center);

        for(auto it : pts) {
            const Eigen::Vector3d point = it - center;
            calc_radius += sqrt(pow(point.x(), 2.0) + pow(point.z(), 2.0));
        }

        calc_radius /= static_cast<double>(pts.size());
    }
}

void CircleCmd::solve_real()
{
    std::vector<Eigen::Vector3d> pts = {};

    if(points.size() >= 3) {
        // Нихрена себе, реальный самолет!
        real_plane.set_actual(points);

        for(auto it : points) {
            // Project every single point onto the plane
            pts.push_back(real_plane.project2d(it->get_real_point()));
        }

        const double X12 = pts[0].x() - pts[1].x();
        const double X13 = pts[0].x() - pts[2].x();
        const double Y12 = pts[0].z() - pts[1].z();
        const double Y13 = pts[0].z() - pts[2].z();
        const double Y31 = pts[2].z() - pts[0].z();
        const double Y21 = pts[1].z() - pts[0].z();
        const double X31 = pts[2].x() - pts[0].x();
        const double X21 = pts[1].x() - pts[0].x();

        const double SX13 = pow(pts[0].x(), 2.0) - pow(pts[2].x(), 2.0);
        const double SY13 = pow(pts[0].z(), 2.0) - pow(pts[2].z(), 2.0);
        const double SX21 = pow(pts[1].x(), 2.0) - pow(pts[0].x(), 2.0);
        const double SY21 = pow(pts[1].z(), 2.0) - pow(pts[0].z(), 2.0);

        const double f = ((SX13 * X12) + (SY13 * X12) + (SX21 * X13) + (SY21 * X13)) / (2.0 * ((Y31 * X12) - (Y21 * X13)));
        const double g = ((SX13 * Y12) + (SY13 * Y12) + (SX21 * Y13) + (SY21 * Y13)) / (2.0 * ((X31 * Y12) - (X21 * Y13)));
        const double c = -1.0 * pow(pts[0].x(), 2.0) - pow(pts[0].x(), 2.0) - 2.0 * g * pts[0].x() - 2.0 * f * pts[0].z();

        // Circle's center
        const Eigen::Vector3d center = {-g, 0.0, -f};
        real_center = real_plane.unproject2d(center);

        for(auto it : pts) {
            const Eigen::Vector3d point = it - center;
            real_radius += sqrt(pow(point.x(), 2.0) + pow(point.z(), 2.0));
        }

        real_radius /= static_cast<double>(pts.size());
    }
}
