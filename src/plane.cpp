#include <plane.hpp>

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

        const Eigen::Vector3d AB = pts[1] - pts[0];
        const Eigen::Vector3d AC = pts[2] - pts[0];
        normal = AB.cross(AC).normalized();

        const Eigen::Vector3d U = AB.normalized();
        const Eigen::Vector3d V = AB.normalized().cross(normal);

        const Eigen::Vector3d bA = pts[0];
        const Eigen::Vector3d bU = pts[0] + U;
        const Eigen::Vector3d bV = pts[0] + V;
        const Eigen::Vector3d bN = pts[0] + normal;

        matrix(0x00) = bA.x();
        matrix(0x01) = bU.x();
        matrix(0x02) = bV.x();
        matrix(0x03) = bN.x();

        matrix(0x04) = bA.y();
        matrix(0x05) = bU.y();
        matrix(0x06) = bV.y();
        matrix(0x07) = bN.y();

        matrix(0x08) = bA.z();
        matrix(0x09) = bU.z();
        matrix(0x0A) = bV.z();
        matrix(0x0B) = bN.z();

        matrix(0x0C) = 1.0;
        matrix(0x0D) = 1.0;
        matrix(0x0E) = 1.0;
        matrix(0x0F) = 1.0;

        invmatrix = matrix.inverse();

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

        const Eigen::Vector3d AB = pts[1] - pts[0];
        const Eigen::Vector3d AC = pts[2] - pts[0];
        normal = AB.cross(AC).normalized();

        const Eigen::Vector3d U = AB.normalized();
        const Eigen::Vector3d V = AB.normalized().cross(normal);

        const Eigen::Vector3d bA = pts[0];
        const Eigen::Vector3d bU = pts[0] + U;
        const Eigen::Vector3d bV = pts[0] + V;
        const Eigen::Vector3d bN = pts[0] + normal;

        matrix(0x00) = bA.x();
        matrix(0x01) = bU.x();
        matrix(0x02) = bV.x();
        matrix(0x03) = bN.x();

        matrix(0x04) = bA.y();
        matrix(0x05) = bU.y();
        matrix(0x06) = bV.y();
        matrix(0x07) = bN.y();

        matrix(0x08) = bA.z();
        matrix(0x09) = bU.z();
        matrix(0x0A) = bV.z();
        matrix(0x0B) = bN.z();

        matrix(0x0C) = 1.0;
        matrix(0x0D) = 1.0;
        matrix(0x0E) = 1.0;
        matrix(0x0F) = 1.0;

        invmatrix = matrix.inverse();

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

#include <iostream>
Eigen::Vector2d Plane::project2d(const Eigen::Vector3d &pv) const
{
    const Eigen::Vector3d pvp = project3d(pv);
    const Eigen::Vector4d pvx = Eigen::Vector4d{pvp.x(), pvp.y(), pvp.z(), 1.0};
    const Eigen::Vector4d tx = matrix * pvx;
    return Eigen::Vector2d{tx.x(), tx.y()};
}

Eigen::Vector3d Plane::unproject2d(const Eigen::Vector2d &uv) const
{
    
}
