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
            const Eigen::Vector3d &point = it->get_calc_point();
            const Eigen::Vector3d pv = calc_plane.project2d(point);
            pts.push_back(pv);
        }

        const Eigen::Vector3d AB {pts[1] - pts[0]};
        const Eigen::Vector3d AC {pts[2] - pts[0]};
        const Eigen::Vector3d BC {pts[2] - pts[1]};

        const double LAB = AB.norm();
        const double LAC = AC.norm();
        const double LBC = BC.norm();

        const double halfp = 0.5 * (LAB + LAC + LBC);
        const double area = sqrt(halfp * (halfp - LAB) * (halfp - LAC) * (halfp - LBC));

        calc_radius = (LAB * LAC * LBC) / (4.0 * area);

        const double E = AB.x() * (pts[0].x() + pts[1].x()) + AB.z() * (pts[0].z() + pts[1].z());
        const double F = AC.x() * (pts[0].x() + pts[2].x()) + AC.z() * (pts[0].z() + pts[2].z());
        const double g = 2.0 * (AB.x() * BC.z() - AB.z() * BC.x());
        const double cx = (AC.z() * E - AB.z() * F) / g;
        const double cz = (AB.x() * F - AC.x() * E) / g;

        calc_center = calc_plane.unproject2d(Eigen::Vector3d{cx, pts[0].y(), cz});
    }
}

void CircleCmd::solve_real()
{
    std::vector<Eigen::Vector3d> pts = {};

    if(points.size() >= 3) {
        // Нихрена себе, реальный самолет!
        real_plane.set_guess(points);

        for(auto it : points) {
            const Eigen::Vector3d &point = it->get_real_point();
            const Eigen::Vector3d pv = real_plane.project2d(point);
            pts.push_back(pv);
        }

        const Eigen::Vector3d AB {pts[1] - pts[0]};
        const Eigen::Vector3d AC {pts[2] - pts[0]};
        const Eigen::Vector3d BC {pts[2] - pts[1]};

        const double LAB = AB.norm();
        const double LAC = AC.norm();
        const double LBC = BC.norm();

        const double halfp = 0.5 * (LAB + LAC + LBC);
        const double area = sqrt(halfp * (halfp - LAB) * (halfp - LAC) * (halfp - LBC));

        real_radius = (LAB * LAC * LBC) / (4.0 * area);

        const double E = AB.x() * (pts[0].x() + pts[1].x()) + AB.z() * (pts[0].z() + pts[1].z());
        const double F = AC.x() * (pts[0].x() + pts[2].x()) + AC.z() * (pts[0].z() + pts[2].z());
        const double g = 2.0 * (AB.x() * BC.z() - AB.z() * BC.x());
        const double cx = (AC.z() * E - AB.z() * F) / g;
        const double cz = (AB.x() * F - AC.x() * E) / g;

        real_center = real_plane.unproject2d(Eigen::Vector3d{cx, pts[0].y(), cz});
    }
}

void CircleCmd::on_load(std::ifstream &file)
{
    size_t offset;
    std::string line {};

    while(std::getline(file, line)) {
        if((offset = line.find("name")) != std::string::npos) {
            name = line.substr(offset + 4);
            continue;
        }

        if((offset = line.find("pcounter")) != std::string::npos) {
            my_pcounter = static_cast<size_t>(strtoull(line.substr(offset + 8).c_str(), nullptr, 10));
            continue;
        }

        if((offset = line.find("point")) != std::string::npos) {
            const size_t pc = static_cast<size_t>(strtoull(line.substr(offset + 5).c_str(), nullptr, 10));

            if(pc < my_pcounter) {
                for(const auto it : globals::commands) {
                    if(it->get_pcounter() == pc && it->get_type() == CmdType::MeasurePoint) {
                        points.insert(reinterpret_cast<const PointCmd *>(it));
                    }
                }
            }

            continue;
        }

        if(line.find("end") != std::string::npos) {
            break;
        }
    }

    solve_calc();
}

void CircleCmd::on_save(std::ofstream &file) const
{
    file << "circle" << std::endl;
    if(!name.empty())
        file << "name" << name << std::endl;
    file << "pcounter" << my_pcounter << std::endl;
    for(const auto it : points)
        file << "point" << it->get_pcounter() << std::endl;
    file << "end" << std::endl;
}
