#ifndef BBE25704_3C2A_468C_AC2C_173A7DB169A4
#define BBE25704_3C2A_468C_AC2C_173A7DB169A4
#include <Eigen/Dense>
#include <unordered_set>
#include <cmd_point.hpp>

class Plane final {
public:
    Plane() = default;
    virtual ~Plane() = default;

    void set(const Eigen::Vector3d &pv, const Eigen::Vector3d &nv);
    bool set_guess(const std::unordered_set<const PointCmd *> &points);
    bool set_actual(const std::unordered_set<const PointCmd *> &points);

    const Eigen::Vector3d &get_point() const;
    const Eigen::Vector3d &get_normal() const;

    Eigen::Vector3d project3d(const Eigen::Vector3d &pv) const;
    Eigen::Vector2d project2d(const Eigen::Vector3d &pv) const;
    Eigen::Vector3d unproject2d(const Eigen::Vector2d &pv) const;

private:
    Eigen::Vector3d point {};
    Eigen::Vector3d normal {};
    Eigen::Matrix<float, 2, 3> matrix {};
};

#endif/* BBE25704_3C2A_468C_AC2C_173A7DB169A4 */
