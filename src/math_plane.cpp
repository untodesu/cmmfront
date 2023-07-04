#include <math_plane.hpp>

bool math::plane_calc(const std::unordered_set<const PointCmd *> &points, Eigen::Vector3d &point, Eigen::Vector3d &normal)
{
    Eigen::Vector3d pts[3];

    normal.setZero();
    point.setZero();

    if(points.size() >= 3) {
        for(auto it : points)
            point += it->get_calc_point();
        point /= static_cast<double>(points.size());

        size_t index = 0;
        for(auto it : points) {
            if(index >= 3)
                break;
            pts[index++] = it->get_calc_point();
        }

        Eigen::Vector3d a = pts[1] - pts[0];
        Eigen::Vector3d b = pts[2] - pts[0];
        normal = a.cross(b).normalized();

        return true;
    }

    return false;
}

bool math::plane_real(const std::unordered_set<const PointCmd *> &points, Eigen::Vector3d &point, Eigen::Vector3d &normal)
{
    Eigen::Vector3d pts[3];

    normal.setZero();
    point.setZero();

    if(points.size() >= 3) {
        for(auto it : points)
            point += it->get_real_point();
        point /= static_cast<double>(points.size());

        size_t index = 0;
        for(auto it : points) {
            if(index >= 3)
                break;
            pts[index++] = it->get_real_point();
        }

        Eigen::Vector3d a = pts[1] - pts[0];
        Eigen::Vector3d b = pts[2] - pts[0];
        normal = a.cross(b).normalized();

        return true;
    }

    return false;
}

Eigen::Vector3d math::plane_proj(const Eigen::Vector3d &offset, const Eigen::Vector3d &normal, const Eigen::Vector3d &point)
{
    const Eigen::Vector3d v = point - offset;
    const float dist = v.dot(normal);
    return point - dist * normal;
}