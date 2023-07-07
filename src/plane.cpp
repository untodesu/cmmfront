#include <Eigen/Geometry>
#include <iostream>
#include <plane.hpp>

static Eigen::Matrix4d look_at_rh(const Eigen::Vector3d &eye, const Eigen::Vector3d &center)
{
    const Eigen::Vector3d UP = {0.0, 1.0, 0.0};
    const Eigen::Vector3d f = (center - eye).normalized();
    const Eigen::Vector3d s = f.cross(UP).normalized();
    const Eigen::Vector3d u = s.cross(f);

    Eigen::Matrix4d result = Eigen::Matrix4d::Identity();
    result.row(0) = Eigen::Vector4d{s.x(), s.y(), s.z(), 0.0};
    result.row(1) = Eigen::Vector4d{u.x(), u.y(), u.z(), 0.0};
    result.row(2) = Eigen::Vector4d{-f.x(), -f.y(), -f.z(), 0.0};
    result.row(3) = Eigen::Vector4d{-s.dot(eye), -u.dot(eye), f.dot(eye), 1.0};

    return result;
}

bool Plane::set_guess(const std::unordered_set<const PointCmd *> &points)
{
    constexpr static const size_t A = 0;
    constexpr static const size_t B = 1;
    constexpr static const size_t C = 2;
    Eigen::Vector3d pts[3]{};

    point.setZero();
    normal.setZero();
    tmatrix.setZero();
    imatrix.setZero();

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

        const Eigen::Vector3d AB = (pts[B] - pts[A]);
        const Eigen::Vector3d AC = (pts[C] - pts[A]);
        normal = AB.cross(AC).normalized();

        // Basis vectors within the plane
        const Eigen::Vector3d U = AB.normalized();
        const Eigen::Vector3d V = AB.normalized().cross(normal).normalized();

        // Projection matrix that turns a 3D point
        // into a 2D point on a plane is made from
        // a matrix MX (columnts: U, normal, V) turned
        // into a minor matrix MM, divided by MX's determinant.
        Eigen::Matrix2d minor = {};
        Eigen::Matrix3d MM = {};
        Eigen::Matrix3d MX = {};

        // M11
        minor.row(0) = Eigen::Vector2d{normal.y(), V.y()};
        minor.row(1) = Eigen::Vector2d{normal.z(), V.z()};
        MM.row(0).x() = minor.determinant();

        // M12
        minor.row(0) = Eigen::Vector2d{U.y(), V.y()};
        minor.row(1) = Eigen::Vector2d{U.z(), V.z()};
        MM.row(0).y() = minor.determinant();

        // M13
        minor.row(0) = Eigen::Vector2d{U.y(), normal.y()};
        minor.row(1) = Eigen::Vector2d{U.z(), normal.z()};
        MM.row(0).z() = minor.determinant();

        // M21
        minor.row(0) = Eigen::Vector2d{normal.x(), V.x()};
        minor.row(1) = Eigen::Vector2d{normal.z(), V.z()};
        MM.row(1).x() = minor.determinant();

        // M22
        minor.row(0) = Eigen::Vector2d{U.x(), V.x()};
        minor.row(1) = Eigen::Vector2d{U.z(), V.z()};
        MM.row(1).y() = minor.determinant();

        // M23
        minor.row(0) = Eigen::Vector2d{U.x(), normal.x()};
        minor.row(1) = Eigen::Vector2d{U.z(), normal.z()};
        MM.row(1).z() = minor.determinant();

        // M31
        minor.row(0) = Eigen::Vector2d{normal.x(), V.x()};
        minor.row(1) = Eigen::Vector2d{normal.y(), V.y()};
        MM.row(2).x() = minor.determinant();

        // M32
        minor.row(0) = Eigen::Vector2d{U.x(), V.x()};
        minor.row(1) = Eigen::Vector2d{U.y(), V.y()};
        MM.row(2).y() = minor.determinant();

        // M33
        minor.row(0) = Eigen::Vector2d{U.x(), normal.x()};
        minor.row(1) = Eigen::Vector2d{U.y(), normal.y()};
        MM.row(2).z() = minor.determinant();

        // MX and its determinant
        MX.col(0) = U;
        MX.col(1) = normal;
        MX.col(2) = V;
        const double MXd = MX.determinant();

        tmatrix = MM.transpose() / MXd;
        imatrix = MX;

        return false;
    }

    return true;
}

bool Plane::set_actual(const std::unordered_set<const PointCmd *> &points)
{
    constexpr static const size_t A = 0;
    constexpr static const size_t B = 1;
    constexpr static const size_t C = 2;
    Eigen::Vector3d pts[3]{};

    point.setZero();
    normal.setZero();
    tmatrix.setZero();
    imatrix.setZero();

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

        const Eigen::Vector3d AB = (pts[B] - pts[A]);
        const Eigen::Vector3d AC = (pts[C] - pts[A]);
        normal = AB.cross(AC).normalized();

        // Basis vectors within the plane
        const Eigen::Vector3d U = AB.normalized();
        const Eigen::Vector3d V = AB.normalized().cross(normal).normalized();

        // Projection matrix that turns a 3D point
        // into a 2D point on a plane is made from
        // a matrix MX (columnts: U, normal, V) turned
        // into a minor matrix MM, divided by MX's determinant.
        Eigen::Matrix2d minor = {};
        Eigen::Matrix3d MM = {};
        Eigen::Matrix3d MX = {};

        // M11
        minor.row(0) = Eigen::Vector2d{normal.y(), V.y()};
        minor.row(1) = Eigen::Vector2d{normal.z(), V.z()};
        MM.row(0).x() = minor.determinant();

        // M12
        minor.row(0) = Eigen::Vector2d{U.y(), V.y()};
        minor.row(1) = Eigen::Vector2d{U.z(), V.z()};
        MM.row(0).y() = minor.determinant();

        // M13
        minor.row(0) = Eigen::Vector2d{U.y(), normal.y()};
        minor.row(1) = Eigen::Vector2d{U.z(), normal.z()};
        MM.row(0).z() = minor.determinant();

        // M21
        minor.row(0) = Eigen::Vector2d{normal.x(), V.x()};
        minor.row(1) = Eigen::Vector2d{normal.z(), V.z()};
        MM.row(1).x() = minor.determinant();

        // M22
        minor.row(0) = Eigen::Vector2d{U.x(), V.x()};
        minor.row(1) = Eigen::Vector2d{U.z(), V.z()};
        MM.row(1).y() = minor.determinant();

        // M23
        minor.row(0) = Eigen::Vector2d{U.x(), normal.x()};
        minor.row(1) = Eigen::Vector2d{U.z(), normal.z()};
        MM.row(1).z() = minor.determinant();

        // M31
        minor.row(0) = Eigen::Vector2d{normal.x(), V.x()};
        minor.row(1) = Eigen::Vector2d{normal.y(), V.y()};
        MM.row(2).x() = minor.determinant();

        // M32
        minor.row(0) = Eigen::Vector2d{U.x(), V.x()};
        minor.row(1) = Eigen::Vector2d{U.y(), V.y()};
        MM.row(2).y() = minor.determinant();

        // M33
        minor.row(0) = Eigen::Vector2d{U.x(), normal.x()};
        minor.row(1) = Eigen::Vector2d{U.y(), normal.y()};
        MM.row(2).z() = minor.determinant();

        // MX and its determinant
        MX.col(0) = U;
        MX.col(1) = normal;
        MX.col(2) = V;
        const double MXd = MX.determinant();

        tmatrix = MM.transpose() / MXd;
        imatrix = MX;

        return false;
    }

    return true;
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
    return tmatrix * project3d(pv);
}

Eigen::Vector3d Plane::unproject2d(const Eigen::Vector3d &uv) const
{
    return imatrix * uv;
}
