#include <Eigen/Geometry>
#include <iostream>
#include <plane.hpp>

bool Plane::set_guess(const std::unordered_set<const PointCmd *> &points)
{
    constexpr static const size_t A = 0;
    constexpr static const size_t B = 1;
    constexpr static const size_t C = 2;
    std::vector<Eigen::Vector3d> pts = {};
    double average_dot = 0.0;

    point.setZero();
    normal.setZero();
    tmatrix.setZero();
    imatrix.setZero();

    if (points.size() >= 3) {
        for (auto it : points) {
            pts.push_back(it->get_calc_point());
        }

        point = pts[0];

        const Eigen::Vector3d AB = (pts[B] - pts[A]);
        const Eigen::Vector3d AC = (pts[C] - pts[A]);
        normal = AB.cross(AC).normalized();

        // Basis vectors within the plane
        const Eigen::Vector3d U = AB.normalized();
        const Eigen::Vector3d V = AB.normalized().cross(normal);

        for(auto it : points)
            average_dot += normal.dot(it->get_calc_normal());
        average_dot /= static_cast<double>(points.size());

        if(average_dot < 0.0) {
            // Negative dot product signifies further
            // calculations being physically unjust,
            // so we flip the normal.
            normal *= -1.0;
        }

        imatrix.col(0) = U;
        imatrix.col(1) = normal;
        imatrix.col(2) = V;

        tmatrix = imatrix.inverse();

        return true;
    }

    return false;
}

bool Plane::set_actual(const std::unordered_set<const PointCmd *> &points)
{
    constexpr static const size_t A = 0;
    constexpr static const size_t B = 1;
    constexpr static const size_t C = 2;
    std::vector<Eigen::Vector3d> pts = {};
    double average_dot = 0.0;

    point.setZero();
    normal.setZero();
    tmatrix.setZero();
    imatrix.setZero();

    if (points.size() >= 3) {
        for (auto it : points) {
            pts.push_back(it->get_real_point());
        }

        point = pts[0];

        const Eigen::Vector3d AB = (pts[B] - pts[A]);
        const Eigen::Vector3d AC = (pts[C] - pts[A]);
        normal = AB.cross(AC).normalized();

        // Basis vectors within the plane
        const Eigen::Vector3d U = AB.normalized();
        const Eigen::Vector3d V = AB.normalized().cross(normal);

        for(auto it : points)
            average_dot += normal.dot(it->get_real_normal());
        average_dot /= static_cast<double>(points.size());

        if(average_dot < 0.0) {
            // Negative dot product signifies further
            // calculations being physically unjust,
            // so we flip the normal.
            normal *= -1.0;
        }

        imatrix.col(0) = U;
        imatrix.col(1) = normal;
        imatrix.col(2) = V;

        tmatrix = imatrix.inverse();

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

Eigen::Vector3d Plane::project2d(const Eigen::Vector3d &pv) const
{
    return tmatrix * pv;
}

Eigen::Vector3d Plane::unproject2d(const Eigen::Vector3d &uv) const
{
    return imatrix * uv;
}
