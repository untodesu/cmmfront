#include <plane.hpp>

void Plane::set(const Eigen::Vector3d &pv, const Eigen::Vector3d &nv)
{
    point = pv;
    normal = nv.normalized();
    const Eigen::Vector3d unit_x = point.normalized();
    const Eigen::Vector3d unit_y = unit_x.cross(normal).normalized();
    matrix(0) = unit_x.x();
    matrix(1) = unit_x.y();
    matrix(2) = unit_x.z();
    matrix(3) = unit_y.x();
    matrix(4) = unit_y.y();
    matrix(5) = unit_y.z();
}

bool Plane::set_guess(const std::unordered_set<const PointCmd *> &points)
{
    Eigen::Vector3d pts[3];

    point.setZero();
    normal.setZero();

    if(points.size() >= 3) {
        for(auto it : points)
            point += it->get_calc_point();
        point /= static_cast<double>(points.size());

        size_t idx = 0;
        for(auto it : points) {
            if(idx >= 3)
                break;
            pts[idx++] = it->get_calc_point();
        }

        const Eigen::Vector3d u = pts[1] - pts[0];
        const Eigen::Vector3d v = pts[2] - pts[0];
        normal = u.cross(v).normalized();

        const Eigen::Vector3d unit_x = u.normalized();
        const Eigen::Vector3d unit_y = unit_x.cross(normal);
        matrix(0) = unit_x.x();
        matrix(1) = unit_x.y();
        matrix(2) = unit_x.z();
        matrix(3) = unit_y.x();
        matrix(4) = unit_y.y();
        matrix(5) = unit_y.z();

        return true;
    }

    return false;
}

bool Plane::set_actual(const std::unordered_set<const PointCmd *> &points)
{
    Eigen::Vector3d pts[3];

    point.setZero();
    normal.setZero();

    if(points.size() >= 3) {
        for(auto it : points)
            point += it->get_real_point();
        point /= static_cast<double>(points.size());

        size_t idx = 0;
        for(auto it : points) {
            if(idx >= 3)
                break;
            pts[idx++] = it->get_real_point();
        }

        const Eigen::Vector3d u = pts[1] - pts[0];
        const Eigen::Vector3d v = pts[2] - pts[0];
        normal = u.cross(v).normalized();

        const Eigen::Vector3d unit_x = u.normalized();
        const Eigen::Vector3d unit_y = unit_x.cross(normal);
        matrix(0) = unit_x.x();
        matrix(1) = unit_x.y();
        matrix(2) = unit_x.z();
        matrix(3) = unit_y.x();
        matrix(4) = unit_y.y();
        matrix(5) = unit_y.z();

        return true;
    }

    return false;
}

const Eigen::Vector3d &Plane::get_point() const
{
    return point;
}

const Eigen::Vector3d &Plane::get_normal() const
{
    return normal;
}

Eigen::Vector3d Plane::project3d(const Eigen::Vector3d &pv) const
{
    const Eigen::Vector3d vec = pv - point;
    const float dist = vec.dot(normal);
    return pv - normal * dist;
}

Eigen::Vector2d Plane::project2d(const Eigen::Vector3d &pv) const
{
    const float x = matrix(0) * pv.x() + matrix(1) * pv.y() + matrix(2) * pv.z();
    const float y = matrix(3) * pv.x() + matrix(4) * pv.y() + matrix(5) * pv.z();
    return Eigen::Vector2d{x, y};
}

Eigen::Vector3d Plane::unproject2d(const Eigen::Vector2d &uv) const
{
    
}
