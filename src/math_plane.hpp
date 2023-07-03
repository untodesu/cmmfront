#ifndef BBE25704_3C2A_468C_AC2C_173A7DB169A4
#define BBE25704_3C2A_468C_AC2C_173A7DB169A4
#include <Eigen/Dense>
#include <unordered_set>
#include <cmd_point.hpp>

namespace math
{
bool plane_calc(const std::unordered_set<const PointCmd *> &points, Eigen::Vector3d &offset, Eigen::Vector3d &normal);
bool plane_real(const std::unordered_set<const PointCmd *> &points, Eigen::Vector3d &offset, Eigen::Vector3d &normal);
Eigen::Vector3d plane_proj(const Eigen::Vector3d &offset, const Eigen::Vector3d &normal, const Eigen::Vector3d &point);
} // namespace math

#endif/* BBE25704_3C2A_468C_AC2C_173A7DB169A4 */
