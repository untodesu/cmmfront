#include <cmd_circle.hpp>
#include <cmd_point.hpp>
#include <cmd_plane.hpp>
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

const Eigen::Vector3d &PointCmd::get_calc_normal() const
{
    return calc_normal;
}

const Eigen::Vector3d &PointCmd::get_calc_point() const
{
    return calc_point;
}

const Eigen::Vector3d &PointCmd::get_real_normal() const
{
    return real_normal;
}

const Eigen::Vector3d &PointCmd::get_real_point() const
{
    return real_point;
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
            cmm->cmd_point(calc_point, calc_normal, real_point);
            real_normal = calc_normal;
            break;
        case PointType::PlaneProj:
            calc_point = target_plane->get_calc_plane().project3d(calc_proj_target);
            calc_normal = target_plane->get_calc_plane().get_normal();
            real_point = target_plane->get_real_plane().project3d(calc_proj_target);
            real_normal = target_plane->get_real_plane().get_normal();
            break;
        case PointType::CircleCenter:
            calc_point = target_circle->get_calc_point();
            calc_normal = target_circle->get_calc_plane().get_normal();
            real_point = target_circle->get_real_point();
            real_normal = target_circle->get_real_plane().get_normal();
            break;
    }
}

void PointCmd::on_draw_imgui()
{
    float temp[3];
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

    int index = static_cast<int>(point_type);
    if(ImGui::Combo("Type", &index, "Generic\0Plane Projection\0Circle Center\0")) {
        // A new point type
        point_type = static_cast<PointType>(index);
    }

    if(point_type == PointType::Generic) {
        temp[0] = calc_point.x();
        temp[1] = calc_point.y();
        temp[2] = calc_point.z();
        if(ImGui::InputFloat3("Point", temp)) {
            calc_point = Eigen::Vector3d{temp[0], temp[1], temp[2]};
            real_point = Eigen::Vector3d{};
        }

        temp[0] = calc_normal.x();
        temp[1] = calc_normal.y();
        temp[2] = calc_normal.z();
        if(ImGui::InputFloat3("Normal", temp)) {
            calc_normal = Eigen::Vector3d{temp[0], temp[1], temp[2]};
            real_point = Eigen::Vector3d{};
        }
    }

    if(point_type == PointType::PlaneProj) {
        temp[0] = calc_proj_target.x();
        temp[1] = calc_proj_target.y();
        temp[2] = calc_proj_target.z();
        if(ImGui::InputFloat3("Target", temp)) {
            calc_proj_target = Eigen::Vector3d{temp[0], temp[1], temp[2]};
            if(target_plane)
                calc_point =  target_plane->get_calc_plane().project3d(calc_proj_target);
            else
                calc_point = Eigen::Vector3d{};
            real_point = Eigen::Vector3d{};
        }
    }

    if(point_type == PointType::PlaneProj) {
        if(ImGui::ListBoxHeader("Plane")) {
            for(auto it : globals::commands) {
                if(it->get_pcounter() < my_pcounter && it->get_type() == CmdType::MeasurePlane) {
                    PlaneCmd *pcmd = reinterpret_cast<PlaneCmd *>(it);
                    bool selected = (target_plane == pcmd);
                    snprintf(stager, sizeof(stager), "%s [%04zX]", it->get_name().c_str(), it->get_pcounter());
                    if(!ImGui::Selectable(stager, &selected))
                        continue;
                    target_plane = pcmd;
                    calc_point = target_plane->get_calc_plane().project3d(calc_proj_target);
                    real_point = Eigen::Vector3d{};
                }
            }

            ImGui::ListBoxFooter();
        }
    }

    if(point_type == PointType::CircleCenter) {
        if(ImGui::ListBoxHeader("Circle")) {
            for(auto it : globals::commands) {
                if(it->get_pcounter() < my_pcounter && it->get_type() == CmdType::MeasureCircle) {
                    CircleCmd *ccmd = reinterpret_cast<CircleCmd *>(it);
                    bool selected = (target_circle == ccmd);
                    snprintf(stager, sizeof(stager), "%s [%04zX]", it->get_name().c_str(), it->get_pcounter());
                    if(!ImGui::Selectable(stager, &selected))
                        continue;
                    target_circle = ccmd;
                    calc_point = target_circle->get_calc_point();
                    real_point = Eigen::Vector3d{};
                }
            }

            ImGui::ListBoxFooter();
        }
    }

    ImGui::Text("Guessed point: %.3f %.3f %.3f", calc_point.x(), calc_point.y(), calc_point.z());
    ImGui::Text("Actual point: %.3f %.3f %.3f", real_point.x(), real_point.y(), real_point.z());
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

    if(point_type == PointType::CircleCenter) {
        if(target_circle) {
            for(auto it : globals::commands) {
                if(it != target_circle)
                    continue;
                return true;
            }

            // Invalid pointer
            return false;
        }

        // No circles?
        return false;
    }

    return true;
}

void PointCmd::on_load(std::ifstream &file)
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

        if((offset = line.find("ptarget")) != std::string::npos) {
            const size_t pc = static_cast<size_t>(strtoull(line.substr(offset + 7).c_str(), nullptr, 10));

            for(auto it : globals::commands) {
                if(it->get_pcounter() == pc && it->get_type() == CmdType::MeasurePlane) {
                    target_plane = reinterpret_cast<PlaneCmd *>(it);
                    break;
                }
            }

            point_type = PointType::PlaneProj;

            continue;
        }

        if((offset = line.find("ctarget")) != std::string::npos) {
            const size_t pc = static_cast<size_t>(strtoull(line.substr(offset + 7).c_str(), nullptr, 10));

            for(auto it : globals::commands) {
                if(it->get_pcounter() == pc && it->get_type() == CmdType::MeasureCircle) {
                    target_circle = reinterpret_cast<CircleCmd *>(it);
                    break;
                }
            }

            point_type = PointType::CircleCenter;

            continue;
        }

        if((offset = line.find("px")) != std::string::npos) {
            calc_point.x() = atof(line.substr(offset + 2).c_str());
            point_type = PointType::Generic;
            continue;
        }

        if((offset = line.find("py")) != std::string::npos) {
            calc_point.y() = atof(line.substr(offset + 2).c_str());
            point_type = PointType::Generic;
            continue;
        }

        if((offset = line.find("pz")) != std::string::npos) {
            calc_point.z() = atof(line.substr(offset + 2).c_str());
            point_type = PointType::Generic;
            continue;
        }

        if((offset = line.find("nx")) != std::string::npos) {
            calc_normal.x() = atof(line.substr(offset + 2).c_str());
            point_type = PointType::Generic;
            continue;
        }

        if((offset = line.find("ny")) != std::string::npos) {
            calc_normal.y() = atof(line.substr(offset + 2).c_str());
            point_type = PointType::Generic;
            continue;
        }

        if((offset = line.find("nz")) != std::string::npos) {
            calc_normal.z() = atof(line.substr(offset + 2).c_str());
            point_type = PointType::Generic;
            continue;
        }

        if(line.find("end") != std::string::npos) {
            break;
        }
    }
}

void PointCmd::on_save(std::ofstream &file) const
{
    file << "point" << std::endl;
    if(!name.empty())
        file << "name" << name << std::endl;
    file << "pcounter" << my_pcounter << std::endl;
    switch(point_type) {
        case PointType::Generic:
            file << "px" << calc_point.x() << std::endl;
            file << "py" << calc_point.y() << std::endl;
            file << "pz" << calc_point.z() << std::endl;
            file << "nx" << calc_normal.x() << std::endl;
            file << "ny" << calc_normal.y() << std::endl;
            file << "nz" << calc_normal.z() << std::endl;
            break;
        case PointType::PlaneProj:
            if(target_plane)
                file << "ptarget" << target_plane->get_pcounter() << std::endl;
            break;
        case PointType::CircleCenter:
            if(target_circle)
                file << "ctarget" << target_circle->get_pcounter() << std::endl;
            break;
    }
    file << "end" << std::endl;
}
